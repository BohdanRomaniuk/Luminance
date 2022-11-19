#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "ternary_pattern.h"

#define IS_DEBUG_MODE  //Debuging
#define SKIP_CAPTIVE_PORTAL

#ifdef IS_DEBUG_MODE
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif

#define APPLICATION_VERSION 0.2
#define APPLICATION_NAME "Luminance"
#define ACESS_POINT_PASS "12345678"

#define LED_PIN 22
#define LED_TYPE WS2812
#define LED_MAX 150
#define COLOR_ORDER GRB
#define VOLTAGE 5
#define CURRENT_LIMIT 1000  //Maximum current in mA

IPAddress appIpAdress;  //startup.cpp
WebServer server(81);   //server.cpp

CRGB leds[LED_MAX];
CLEDController* strip;
byte brightness = 50;

void setup() {
#ifdef IS_DEBUG_MODE
  Serial.begin(9600);
#endif

  initStrip();
  onStartup();

  server.on("/", HTTP_GET, getAppInfo);
  server.on("/api/getAppInfo", HTTP_GET, getAppInfo);
  server.on("/api/setBrightness", HTTP_POST, setBrightness);
  server.on("/api/startMapping", HTTP_GET, startMapping);
  server.on("/api/getFrame", HTTP_GET, getFrame);
  server.onNotFound(onNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}

void initStrip() {
  strip = &FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_MAX).setCorrection(TypicalLEDStrip);
  if (CURRENT_LIMIT > 0) {
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTAGE, CURRENT_LIMIT);
  }
  FastLED.setBrightness(brightness);
  strip->setLeds(leds, LED_MAX);
  strip->clearLedData();
  fader(CRGB::Orange);
  strip->showLeds(LED_MAX);
}

void getAppInfo() {
  DynamicJsonDocument data(100);
  data["version"] = APPLICATION_VERSION;
  data["name"] = APPLICATION_NAME;
  String response;
  serializeJson(data, response);

  server.send(200, "application/json", response);
}

void setBrightness() {
  if (server.hasArg("plain") == false) {
    return;
  }

  String body = server.arg("plain");
  StaticJsonDocument<30> jsonDocument;
  deserializeJson(jsonDocument, body);
  uint8_t brightnessValue = jsonDocument["brightness"];

  strip->showLeds(brightnessValue);
  LOG(brightnessValue);
}

void startMapping() {
  TernaryPattern pattern(LED_MAX);
  for (int i = 0; i < LED_MAX; ++i) {
    int color = pattern.getFrameColor(i, 0);
    leds[i] = color == 0 ? CRGB::Red : color == 1 ? CRGB::Green
                                                  : CRGB::Blue;
  }
  strip->showLeds(LED_MAX);

  StaticJsonDocument<JSON_ARRAY_SIZE(LED_MAX)> data;
  JsonArray array = data.to<JsonArray>();
  int* result = pattern.getPatternDecimal();
  for (int i = 0; i < LED_MAX; ++i) {
    array.add(result[i]);
  }

  String response;
  serializeJson(data, response);

  server.send(200, "application/json", response);
}

void getFrame() {
}

void onNotFound() {
  server.send(404, "application/json", "{\"message\":\"Not Found\"}");
}