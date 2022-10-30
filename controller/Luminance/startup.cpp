#include "startup.h"

IPAddress appIpAdress;

void onStartup()
{
  LOG("App starting...");
  startCaptivePortal();
  while(!isCaptivePortalConfigured())
  {
    //TODO: Blink with blue color
    LOG("Waiting for user");
  }

  appIpAdress = isAccessPoint() ? setupAcessPoint() : setupStationMode();
  LOG(appIpAdress);
}

IPAddress setupAcessPoint() {
  LOG("Acess Point Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APPLICATION_NAME, ACESS_POINT_PASS);
  //TODO: Blink with green color

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
      //TODO: Blink with green color

      return WiFi.localIP();
    }
    //TODO: Blink with yellow color
    yield();
  }
  //TODO: Blink with red color
  LOG("Failed connecting to Wifi");

  return setupAcessPoint();
}
