#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define IS_DEBUG_MODE  //Debuging

#ifdef IS_DEBUG_MODE
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif

#define APPLICATION_VERSION 0.1
#define APPLICATION_NAME "Luminance"
#define ACESS_POINT_PASS "12345678"

#define LED_PIN 22
#define LED_TYPE WS2812
#define LED_MAX 500
#define COLOR_ORDER GRB
#define VOLTAGE 5
#define CURRENT_LIMIT 1000 //Maximum current in mA


IPAddress appIpAdress;  //startup.cpp

WebServer server(81);  //server.cpp

CRGB leds[LED_MAX];
CLEDController *strip;

void setup() {
  initStrip();

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

void initStrip() {
  strip = &FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_MAX).setCorrection(TypicalLEDStrip);
  if (CURRENT_LIMIT > 0)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTAGE, CURRENT_LIMIT);
  }
  strip->setLeds(leds, LED_MAX);
  strip->clearLedData();
  fader(CRGB::Orange);
  strip->showLeds(100);
}

void getAppInfo() {
  DynamicJsonDocument data(100);
  data["version"] = APPLICATION_VERSION;
  data["name"] = APPLICATION_NAME;
  String response;
  serializeJson(data, response);

  server.send(200, "application/json", response);
}

void onNotFound() {
  server.send(404, "application/json", "{\"message\":\"Not Found\"}");
}