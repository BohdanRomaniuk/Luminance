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
  byte _udpBuffer[UDP_PACKET_SIZE];
  Effects* _effects;

  IPAddress setupAcessPoint();
  IPAddress setupStationMode();
public:
  Startup(Effects* effects);

  void onStartup();
  void udpBroadcast();
};

#endif