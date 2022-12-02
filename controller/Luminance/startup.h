#ifndef StartupHeader
#define StartupHeader

#include <WiFi.h>
#include "captive_portal.h"
#include "config.h"
#include "effects.h"

class Startup {
private:
  IPAddress appIpAdress;
  WiFiUDP* _udp;
  byte _udpBuffer[UPD_SECRET_KEY_LENGTH];
  byte _udpResponse[4];
  Effects* _effects;

  IPAddress setupAcessPoint();
  IPAddress setupStationMode();

  bool isAuthorized();
public:
  Startup(Effects* effects);

  void onStartup();
  void udpBroadcast();
};

#endif