#define IS_DEBUG_MODE //Debuging

#ifdef IS_DEBUG_MODE
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif