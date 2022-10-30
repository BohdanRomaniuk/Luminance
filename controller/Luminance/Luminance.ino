#include <FastLED.h>
#include <ArduinoJson.h>
#include "config.h"
#include "captive_portal.h"
#include "startup.h"

WebServer server(80);
IPAddress ipAddress;

CRGB leds[LED_MAX];

void setup() {
#ifdef IS_DEBUG_MODE
  Serial.begin(9600);
#endif
  LOG("App starting...");
  startCaptivePortal();
  while(!isCaptivePortalConfigured())
  {
    LOG("Waiting for user");
  }

  ipAddress = isAccessPoint() ? setupAcessPoint() : setupStationMode();
  LOG(ipAddress);

  server.on("/", HTTP_GET, getAppInfo);
  server.onNotFound(onNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}

void getAppInfo()
{
  DynamicJsonDocument data(100);
  data["version"] = APPLICATION_VERSION;
  data["name"] = APPLICATION_NAME;
  String response;
  serializeJson(data, response);

  server.send(200, "application/json", response);
}

void onNotFound()
{
  server.send(404, "application/json",  "{\"message\":\"Not Found\"}");
}
