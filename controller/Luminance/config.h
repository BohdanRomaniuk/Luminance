#define APPLICATION_NAME "Luminance"
#define APPLICATION_VERSION 0.1
#define ACESS_POINT_PASS "12345678"

#define LED_PIN 2
#define LED_TYPE WS2812
#define LED_ORDER GRB
#define LED_MAX 500

#define IS_DEBUG_MODE //Debuging

#ifdef IS_DEBUG_MODE
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif