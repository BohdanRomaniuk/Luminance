#ifndef LedControllerHeader
#define LedControllerHeader

#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "ternary_pattern.h"
#include "effects.h"

class LedController {
private:
  CRGB _leds[LED_MAX];
  CLEDController* _strip;
  WebServer* _server;
  TernaryPattern* _pattern;
  Effects* _effects;

  void getAppInfo();
  void setBrightness();
  void startMapping();
  void getFrame();
  void onNotFound();
public:
  LedController();
  void startServer();
  void loop();
  void initStrip();

  Effects* getEffects();
};

#endif