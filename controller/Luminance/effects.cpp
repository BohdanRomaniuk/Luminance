#include "effects.h"

Effects::Effects(CLEDController* strip) {
  _strip = strip;
}

void Effects::fader(CRGB color) {
  static uint32_t tmr;
  static int val = 0;
  static bool dir = true;
  if (millis() - tmr >= 20) {
    tmr = millis();
    val += dir ? 3 : -3;
    val = constrain(val, 5, 120);
    if (val >= 120 || val <= 5) dir = !dir;
    _strip->showColor(color, val);
  }
}

void Effects::fadeBlink(CRGB color) {
  for (int i = 0; i < 200; i += 20) {
    _strip->showColor(color, i);
    delay(40);
  }
  for (int i = 200; i > 0; i -= 20) {
    _strip->showColor(color, i);
    delay(40);
  }
  _strip->clearLedData();
  _strip->showLeds(0);
}