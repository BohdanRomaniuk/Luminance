#ifndef LedControllerHeader
#define LedControllerHeader

#include <FastLED.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "ternary_pattern.h"
#include "effects.h"

class LedController {
private:
  CRGB _leds[LED_MAX];
  CLEDController* _strip;
  AsyncWebServer * _server;
  TernaryPattern* _pattern;
  Effects* _effects;

  void showFrame(int id);

  void getAppInfo(AsyncWebServerRequest *request);
  void setBrightness(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
  void startMapping(AsyncWebServerRequest *request);
  void getFrame(AsyncWebServerRequest *request);
  void onNotFound(AsyncWebServerRequest *request);
public:
  LedController();
  void startServer();
  void loop();
  void initStrip();

  Effects* getEffects();
};

#endif