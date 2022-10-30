#include "startup.h"
#include "config.h"
#include "captive_portal.h"

IPAddress setupAcessPoint() {
  LOG("Acess Point Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APPLICATION_NAME, ACESS_POINT_PASS);

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

      return WiFi.localIP();
    }
    yield();
  }
  LOG("Failed connecting to Wifi");

  return setupAcessPoint();
}