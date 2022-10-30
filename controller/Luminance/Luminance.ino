#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "startup.h"

WebServer server(80);
CRGB leds[LED_MAX];

void setup() {
#ifdef IS_DEBUG_MODE
  Serial.begin(9600);
#endif
  onStartup();

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
