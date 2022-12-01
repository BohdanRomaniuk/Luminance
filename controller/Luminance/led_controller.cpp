#include "led_controller.h"

LedController::LedController() {
  _server = new WebServer(WEB_SERVER_PORT);
  _udp = new WiFiUDP();

  _server->on("/", HTTP_GET, std::bind(&LedController::getAppInfo, this));
  _server->on("/api/getAppInfo", HTTP_GET, std::bind(&LedController::getAppInfo, this));
  _server->on("/api/setBrightness", HTTP_POST, std::bind(&LedController::setBrightness, this));
  _server->on("/api/startMapping", HTTP_GET, std::bind(&LedController::startMapping, this));
  _server->on("/api/getFrame", HTTP_GET, std::bind(&LedController::getFrame, this));
  _server->onNotFound(std::bind(&LedController::onNotFound, this));
}

void LedController::startServer() {
  _server->begin();
  _udp->begin(UDP_SERVER_PORT);
}

void LedController::loop() {
  _server->handleClient();
  parseUdp();
}

void LedController::parseUdp() {
  if (_udp->parsePacket()) {
    int n = _udp->read(_udpBuffer, UDP_PACKET_SIZE);
    LOG(n);
    _udpBuffer[n] = 0;
    if (_udpBuffer[0] != 'L') {
      return;
    }

    byte answ[10];
    delay(200);
    answ[0] = 0;
    answ[1] = 199;
    udpReply(answ, 10);
    LOG("REPLIED");
  }
}

void LedController::udpReply(byte* data, byte size) {
  _udp->beginPacket(_udp->remoteIP(), _udp->remotePort());
  _udp->write(data, size);
  _udp->endPacket();
}

Effects* LedController::getEffects() {
  return _effects;
}

void LedController::initStrip() {
  _strip = &FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, LED_MAX).setCorrection(TypicalLEDStrip);
  _effects = new Effects(_strip);
  if (CURRENT_LIMIT > 0) {
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTAGE, CURRENT_LIMIT);
  }
  _strip->setLeds(_leds, LED_MAX);
  _strip->clearLedData();
  //_effects->fader(CRGB::Orange);
  //_strip->showLeds(255);
}

void LedController::showFrame(int id) {
  for (int i = 0; i < LED_MAX; ++i) {
    switch (_pattern->getFrameColor(i, id)) {
      default:
      case 0:
        _leds[i] = CRGB::Red;
        break;
      case 1:
        _leds[i] = CRGB::Green;
        break;
      case 2:
        _leds[i] = CRGB::Blue;
        break;
    }
  }
  _strip->showLeds(LED_MAX);
}

void LedController::getAppInfo() {
  DynamicJsonDocument data(100);
  data["version"] = APPLICATION_VERSION;
  data["name"] = APPLICATION_NAME;
  String response;
  serializeJson(data, response);

  _server->send(200, "application/json", response);
}

void LedController::setBrightness() {
  if (_server->hasArg("plain") == false) {
    _server->send(200, "application/json", "{\"success\":false}");
    return;
  }

  String body = _server->arg("plain");
  StaticJsonDocument<30> jsonDocument;
  deserializeJson(jsonDocument, body);
  uint8_t brightnessValue = constrain(jsonDocument["brightness"], 0, 255);

  _strip->showLeds(brightnessValue);
  _server->send(200, "application/json", "{\"success\":true}");
}

void LedController::startMapping() {
  _pattern = new TernaryPattern(LED_MAX);
  showFrame(0);

  StaticJsonDocument<JSON_ARRAY_SIZE(LED_MAX + 1)> data;
  JsonArray array = data.to<JsonArray>();
  array.add(_pattern->getPatternLength());
  int* result = _pattern->getPatternDecimal();
  for (int i = 0; i < LED_MAX; ++i) {
    array.add(result[i]);
  }

  String response;
  serializeJson(data, response);
  _server->send(200, "application/json", response);
}

void LedController::getFrame() {
  int id = _server->arg("id") == "" ? -1 : _server->arg("id").toInt();
  if (_pattern == nullptr || id < 0 || id >= _pattern->getPatternLength()) {
    _server->send(200, "application/json", "{\"success\":false}");
    return;
  }

  showFrame(id);
  _server->send(200, "application/json", "{\"success\":true}");
}

void LedController::onNotFound() {
  _server->send(404, "application/json", "{\"message\":\"Not Found\"}");
}
