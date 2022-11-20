#ifndef EffectsHeader
#define EffectsHeader

#include <FastLED.h>

class Effects
{
private:
  CLEDController* _strip;
public:
  Effects(CLEDController* strip);

  void fader(CRGB color);
  void fadeBlink(CRGB color);
};

#endif