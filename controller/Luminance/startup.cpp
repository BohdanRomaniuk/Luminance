#include "startup.h"

Startup::Startup(Effects* effects)
{
  _effects = effects;
}

void Startup::onStartup()
{
  LOG("App starting...");
#ifdef SKIP_CAPTIVE_PORTAL
  strcpy(captivePortalConfig.SSID, OPEN_WIFI_SSID);
  strcpy(captivePortalConfig.password, OPEN_WIFI_PASS);
  appIpAdress = setupStationMode();
#else
  startCaptivePortal();
  while(!isCaptivePortalConfigured()) // Waiting for user interaction
  {
    _effects->fader(CRGB::Cyan);
  }

  appIpAdress = isAccessPoint() ? setupAcessPoint() : setupStationMode();
#endif
  
  LOG(appIpAdress);
}

IPAddress Startup::setupAcessPoint() {
  LOG("Acess Point Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APPLICATION_NAME, ACESS_POINT_PASS);
  _effects->fadeBlink(CRGB::Green);

  return WiFi.softAPIP();
}

IPAddress Startup::setupStationMode() {
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
      _effects->fadeBlink(CRGB::Green);

      return WiFi.localIP();
    }
    _effects->fader(CRGB::Yellow);
    yield();
  }
  _effects->fader(CRGB::Red);
  LOG("Failed connecting to Wifi");

  return setupAcessPoint();
}