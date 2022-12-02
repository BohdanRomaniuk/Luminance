#include "startup.h"

Startup::Startup(Effects* effects)
{
  _udp = new WiFiUDP();
  _effects = effects;
}

void Startup::onStartup()
{
  LOG("App starting...");
#ifdef SKIP_CAPTIVE_PORTAL
  strcpy(captivePortalConfig.SSID, OPEN_WIFI_SSID);
  strcpy(captivePortalConfig.password, OPEN_WIFI_PASS);
  appIpAdress = setupStationMode();
#else
  startCaptivePortal();
  while(!isCaptivePortalConfigured()) // Waiting for user interaction
  {
    _effects->fader(CRGB::Cyan);
  }

  appIpAdress = isAccessPoint() ? setupAcessPoint() : setupStationMode();
#endif
  
  LOG(appIpAdress);
  _udp->begin(UDP_SERVER_PORT);
}

IPAddress Startup::setupAcessPoint() {
  LOG("Acess Point Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APPLICATION_NAME, ACESS_POINT_PASS);
  _effects->fadeBlink(CRGB::Green);

  return WiFi.softAPIP();
}

IPAddress Startup::setupStationMode() {
  LOG("Station Mode");
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(APPLICATION_NAME);
  WiFi.begin(captivePortalConfig.SSID, captivePortalConfig.password);
  uint32_t timer = millis();
  bool state = false;
  while (millis() - timer < 15000) {
    if (WiFi.status() == WL_CONNECTED) {
      LOG("Connected to wifi network successfuly");
      _effects->fadeBlink(CRGB::Green);

      return WiFi.localIP();
    }
    _effects->fader(CRGB::Yellow);
    yield();
  }
  _effects->fader(CRGB::Red);
  LOG("Failed connecting to Wifi");

  return setupAcessPoint();
}

void Startup::udpBroadcast() {
  if (_udp->parsePacket()) {
    int received = _udp->read(_udpBuffer, UDP_PACKET_SIZE);
    if (received <= 0 || _udpBuffer[0] != 'L' || _udpBuffer[1] != 'u' || _udpBuffer[2] != 'm') {
      return;
    }

    byte response[4];
    response[0] = appIpAdress[0];
    response[1] = appIpAdress[1];
    response[2] = appIpAdress[2];
    response[3] = appIpAdress[3];
    _udp->beginPacket(_udp->remoteIP(), _udp->remotePort());
    _udp->write(response, 4);
    _udp->endPacket();
  }
}