#include "config.h"
#include "startup.h"
#include "led_controller.h"

Startup* _startup;
LedController* _ledController;

void setup() {
#ifdef IS_DEBUG_MODE
  Serial.begin(9600);
#endif
  _ledController = new LedController();
  _ledController->initStrip();

  _startup = new Startup(_ledController->getEffects());
  _startup->onStartup();
  _ledController->startServer();
}

void loop() {
  _ledController->loop();
}

