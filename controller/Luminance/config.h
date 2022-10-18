#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

#define CONFIG_NAME "Luminance Config"
#define CONFIG_IP 192,168,1,1

#define CONFIG_ERROR 0
#define CONFIG_SUBMIT 1
#define CONFIG_SWITCH_AP 2
#define CONFIG_SWITCH_LOCAL 3
#define CONFIG_EXIT 4
#define CONFIG_TIMEOUT 5

struct Configuration {
  char SSID[32] = "";
  char pass[32] = "";
  uint8_t mode = WIFI_AP;
};
extern Configuration config;

void configStart();
void configStop();
bool configTick();
void configRun(uint32_t prd = 60000);
byte getConfigStatus();

void handleConnect();
void handleAP();
void handleLocal();
void handleExit();
