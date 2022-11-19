#include "captive_portal.h"

void onStartup()
{
  LOG("App starting...");
#ifdef SKIP_CAPTIVE_PORTAL
  strcpy(captivePortalConfig.SSID, "Oasis 2.0 Guest");
  strcpy(captivePortalConfig.password, "789456123");
  appIpAdress = setupStationMode();
#else
  startCaptivePortal();
  while(!isCaptivePortalConfigured()) // Waiting for user interaction
  {
    fader(CRGB::Cyan);
  }

  appIpAdress = isAccessPoint() ? setupAcessPoint() : setupStationMode();
#endif
  
  LOG(appIpAdress);
}

IPAddress setupAcessPoint() {
  LOG("Acess Point Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APPLICATION_NAME, ACESS_POINT_PASS);
  fadeBlink(CRGB::Green);

  return WiFi.softAPIP();
}

IPAddress setupStationMode() {
  LOG("Station Mode");
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(APPLICATION_NAME);
  WiFi.begin(captivePortalConfig.SSID, captivePortalConfig.password);
  uint32_t timer = millis();
  bool state = false;
  while (millis() - timer < 15000) {
    if (WiFi.status() == WL_CONNECTED) {
      LOG("Connected to wifi network successfuly");
      fadeBlink(CRGB::Green); //TODO: Blink with green color

      return WiFi.localIP();
    }
    fader(CRGB::Yellow); //TODO: Blink with yellow color
    yield();
  }
  fader(CRGB::Red); //TODO: Blink with red color
  LOG("Failed connecting to Wifi");

  return setupAcessPoint();
}
