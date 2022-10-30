#ifndef CaptivePortal
#define CaptivePortal
#define CAPTIVE_PORTAL_NAME "Luminance Config"
#define CAPTIVE_PORTAL_IP 192,168,1,1

#include <DNSServer.h>
#include <WiFi.h>
#include <WebServer.h>

#define INIT_MODE 0
#define WIFI_MODE 1
#define ACCESS_POINT_MODE 2

struct CaptivePortalCfg {
  char SSID[32] = "";
  char password[32] = "";
  uint8_t mode = WIFI_AP;
};
extern CaptivePortalCfg captivePortalConfig;

void startCaptivePortal();
void stopCaptivePortal();
bool isCaptivePortalConfigured();
bool isAccessPoint();
byte getCaptivePortalStatus();

void handleWifiConnect();
void handleAccessPointConnect();
#endif