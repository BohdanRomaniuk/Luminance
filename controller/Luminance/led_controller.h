#ifndef LedControllerHeader
#define LedControllerHeader

#include <FastLED.h>
#include <WiFi.h>
#include <WiFiUdp.h>
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
  WiFiUDP* _udp;
  byte _udpBuffer[UDP_PACKET_SIZE];
  TernaryPattern* _pattern;
  Effects* _effects;

  void showFrame(int id);

  void getAppInfo();
  void setBrightness();
  void startMapping();
  void getFrame();
  void onNotFound();

  void parseUdp();
  void udpReply(byte* data, byte size);
public:
  LedController();
  void startServer();
  void loop();
  void initStrip();

  Effects* getEffects();
};

#endif