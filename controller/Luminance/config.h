#ifndef ConfigHeader
#define ConfigHeader

#include "HardwareSerial.h"

#define IS_DEBUG_MODE  //Debuging
#define SKIP_CAPTIVE_PORTAL

#ifdef IS_DEBUG_MODE
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif

#define APPLICATION_VERSION 0.3
#define APPLICATION_NAME "Luminance"
#define ACESS_POINT_PASS "12345678"

#define OPEN_WIFI_SSID "Oasis 2.0 Guest"
#define OPEN_WIFI_PASS "789456123"

#define LED_PIN 22
#define LED_TYPE WS2812
#define LED_MAX 150
#define COLOR_ORDER GRB
#define VOLTAGE 5
#define CURRENT_LIMIT 1000  //Maximum current in mA

#endif