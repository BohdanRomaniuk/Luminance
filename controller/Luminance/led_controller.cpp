#include "led_controller.h"

LedController::LedController() {
  _server = new WebServer(81);

  _server->on("/", HTTP_GET, std::bind(&LedController::getAppInfo, this));
  _server->on("/api/getAppInfo", HTTP_GET, std::bind(&LedController::getAppInfo, this));
  _server->on("/api/setBrightness", HTTP_POST, std::bind(&LedController::setBrightness, this));
  _server->on("/api/startMapping", HTTP_GET, std::bind(&LedController::startMapping, this));
  _server->on("/api/getFrame", HTTP_GET, std::bind(&LedController::getFrame, this));
  _server->onNotFound(std::bind(&LedController::onNotFound, this));
}

void LedController::startServer() {
  _server->begin();
}

void LedController::loop() {
  _server->handleClient();
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
    return;
  }

  String body = _server->arg("plain");
  StaticJsonDocument<30> jsonDocument;
  deserializeJson(jsonDocument, body);
  uint8_t brightnessValue = jsonDocument["brightness"];

  _strip->showLeds(brightnessValue);
}

void LedController::startMapping() {
  _pattern = new TernaryPattern(LED_MAX);
  for (int i = 0; i < LED_MAX; ++i) {
    int color = _pattern->getFrameColor(i, 0);
    _leds[i] = color == 0 ? CRGB::Red : color == 1 ? CRGB::Green
                                                   : CRGB::Blue;
  }
  _strip->showLeds(LED_MAX);

  StaticJsonDocument<JSON_ARRAY_SIZE(LED_MAX)> data;
  JsonArray array = data.to<JsonArray>();
  int* result = _pattern->getPatternDecimal();
  for (int i = 0; i < LED_MAX; ++i) {
    array.add(result[i]);
  }

  String response;
  serializeJson(data, response);

  _server->send(200, "application/json", response);
}

void LedController::getFrame() {
}

void LedController::onNotFound() {
  _server->send(404, "application/json", "{\"message\":\"Not Found\"}");
}
