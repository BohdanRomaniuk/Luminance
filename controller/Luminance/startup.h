#ifndef StartupHeader
#define StartupHeader

#include <WiFi.h>
#include "captive_portal.h"
#include "config.h"
#include "effects.h"

class Startup {
private:
  IPAddress appIpAdress;
  Effects* _effects;

  IPAddress setupAcessPoint();
  IPAddress setupStationMode();
public:
  Startup(Effects* effects);

  void onStartup();
};

#endif