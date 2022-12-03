#include "led_controller.h"

LedController::LedController() {
  _server = new AsyncWebServer(WEB_SERVER_PORT);
  
  _server->on("/", HTTP_GET, std::bind(&LedController::getAppInfo, this, std::placeholders::_1));
  _server->on("/api/getAppInfo", HTTP_GET, std::bind(&LedController::getAppInfo, this, std::placeholders::_1));
  _server->on("/api/setBrightness", HTTP_POST,[](AsyncWebServerRequest * request){}, NULL, std::bind(&LedController::setBrightness, this,
                  std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3, std::placeholders::_4,
                  std::placeholders::_5));
  _server->on("/api/startMapping", HTTP_GET, std::bind(&LedController::startMapping, this, std::placeholders::_1));
  _server->on("/api/getFrame", HTTP_GET, std::bind(&LedController::getFrame, this, std::placeholders::_1));
  _server->onNotFound(std::bind(&LedController::onNotFound, this, std::placeholders::_1));
}

void LedController::startServer() {
  _server->begin();
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

void LedController::getAppInfo(AsyncWebServerRequest *request) {
  DynamicJsonDocument data(100);
  data["version"] = APPLICATION_VERSION;
  data["name"] = APPLICATION_NAME;
  String response;
  serializeJson(data, response);

  request->send(200, "application/json", response);
}

void LedController::setBrightness(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  if (len == 0) {
    request->send(200, "application/json", "{\"success\":false}");
    return;
  }

  StaticJsonDocument<30> jsonDocument;
  deserializeJson(jsonDocument, (const char*)data);
  uint8_t brightnessValue = constrain(jsonDocument["brightness"], 0, 255);

  _strip->showLeds(brightnessValue);
  request->send(200, "application/json", "{\"success\":true}");
}

void LedController::startMapping(AsyncWebServerRequest *request) {
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
  request->send(200, "application/json", response);
}

void LedController::getFrame(AsyncWebServerRequest *request) {
  int id = !request->hasArg("id") ? -1 : request->arg("id").toInt();
  if (_pattern == nullptr || id < 0 || id >= _pattern->getPatternLength()) {
    request->send(200, "application/json", "{\"success\":false}");
    return;
  }

  showFrame(id);
  request->send(200, "application/json", "{\"success\":true}");
}

void LedController::onNotFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"Not Found\"}");
}
