#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    60
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define VOLTAGE 5
#define CURRENT_LIMIT 3000
#define MODES_AMOUNT 10
#define EFFECT_TIME 10

#define qsuba(x, b)  ((x>b)?x-b:0)

CRGB leds[NUM_LEDS];
CRGBPalette16 gCurrentPalette;
TBlendType currentBlending = LINEARBLEND;
int8_t  currentMode = 0;
uint32_t effectsTimer = 0;

int     thisphase = 0;
int8_t  thisspeed = 4;
int8_t  thisdir = 1;
uint8_t thishue = 0;
uint8_t thisrot = 1;
uint8_t allfreq = 32;
uint8_t thiscutoff = 192;

//two sin
uint8_t thathue = 140;
uint8_t thatrot = 0;
int8_t thatspeed = 4;
int thatphase = 0;
uint8_t thatcutoff = 192;
//two sin

//rainbow march
uint8_t thisdiff = 1;
uint8_t thisindex = 0;
//rainbow march

uint8_t thisfade = 224;
uint8_t thisinc = 1;

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Sunset_Real_gp

};

void setup()
{
  Serial.begin(57600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTAGE, CURRENT_LIMIT);
  }

  gCurrentPalette = gGradientPalettes[0];
}

void loop()
{
  if (millis() - effectsTimer >= EFFECT_TIME * 1000L)
  {
    effectsTimer = millis();
    ++currentMode;
  }
  runEffects();
}

void runEffects()
{
  switch (currentMode)
  {
    case 0:
      blendwave();
      break;
    case 1:
      rainbow_beat();
      break;
    case 2:
      allfreq = 2;
      thisspeed = 1;
      thatspeed = 1;
      thishue = 0;
      thathue = 128;
      thisdir = 1;
      thisrot = 1;
      thatrot = 1;
      thiscutoff = 128;
      thatcutoff = 192;

      two_sin();
      break;
    case 3:
      allfreq = 4;
      thisspeed = -1;
      thatspeed = 0;
      thishue = 64;
      thathue = 192;
      thisdir = 1;
      thisrot = 0;
      thatrot = 0;
      thiscutoff = 64;
      thatcutoff = 192;

      two_sin();
      break;
    case 4:
      allfreq = 6;
      thisspeed = 2;
      thatspeed = 1;
      thishue = 96;
      thathue = 224;
      thisdir = 1;
      thisrot = 0;
      thatrot = 0;
      thiscutoff = 64;
      thatcutoff = 64;

      two_sin();
      break;
    case 5:
      allfreq = 20;
      thisspeed = 2;
      thatspeed = 0;
      thishue = 24;
      thathue = 180;
      thisdir = 1;
      thisrot = 0;
      thatrot = 1;
      thiscutoff = 64;
      thatcutoff = 128;

      two_sin();
      break;
    case 6:
      thisspeed = 2;
      thatspeed = 3;
      thishue = 96;
      thathue = 224;
      thisdir = 1;
      thisrot = 1;
      thatrot = 2;
      thiscutoff = 128;
      thatcutoff = 64;

      two_sin();
      break;
    case 7:
      noise8_pal();
      break;
    case 8:
      thisdir = 1;
      thisrot = 1;
      thisdiff = 1;

      rainbow_march();
      break;
    case 9:
      thisinc = 1;
      thisfade = 2;
      thisdiff = 32;

      confetti_pal();
      break;
    case 10:
      thisdir = -1;
      thisrot = 1;
      thisdiff = 5;
    default:
      currentMode = 0;
      break;
  }
  FastLED.show();
}

void blendwave()
{
  CRGB clr1;
  CRGB clr2;
  uint8_t speed;
  uint8_t loc1;

  if (NUM_LEDS >= 10)
  {
    speed = beatsin8(6, 0, 255);

    clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
    clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

#if NUM_LEDS < 255
    loc1 = beatsin8(10, 0, NUM_LEDS - 1);
#else
    loc1 = beatsin16(10, 0, NUM_LEDS - 1);
#endif

    fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
    fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS - 1, clr1);
  }
}

void rainbow_beat()
{
  if (NUM_LEDS >= 10)
  {
    uint8_t startBeat = beatsin8(17, 0, 255);
    uint8_t endBeat = beatsin8(13, 0, 255);
    fill_rainbow(leds, NUM_LEDS, (startBeat + endBeat) / 2, 8);
  }
}

void two_sin()
{
  if (NUM_LEDS >= 10)
  {
    thisphase += thisspeed * thisdir;
    thatphase += thatspeed * thisdir;

    thishue = thishue + thisrot;
    thathue = thathue + thatrot;

#if NUM_LEDS < 255
    uint8_t k;
#else
    uint16_t k;
#endif

    for (k = 0; k < NUM_LEDS; ++k)
    {
      int thisbright = qsuba(cubicwave8((k * allfreq) + thisphase), thiscutoff);
      int thatbright = qsuba(cubicwave8((k * allfreq) + 128 + thatphase), thatcutoff);

      leds[k] = ColorFromPalette(gCurrentPalette, thishue, thisbright, currentBlending);
      leds[k] += ColorFromPalette(gCurrentPalette, thathue, thatbright, currentBlending);
    }
  }
}

uint16_t dist = 12345;
uint8_t scale = 30;

void noise8_pal()
{
#if NUM_LEDS < 255
  uint8_t i;
#else
  uint16_t i;
#endif
  if (NUM_LEDS >= 10)
  {
    for ( i = 0; i < NUM_LEDS; i++)
    {
      uint8_t index = inoise8(i * scale, dist + i * scale) % 255;
      leds[i] = ColorFromPalette(gCurrentPalette, index, 255, currentBlending);
    }
    dist += beatsin8(10, 1, 4) * thisdir;
  }
}

void rainbow_march()
{
  thisindex += thisrot * thisdir;
  fill_rainbow(leds, NUM_LEDS, thisindex, thisdiff);
}

void confetti_pal()
{
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
#if NUM_LEDS < 255
  uint8_t pos = random8(NUM_LEDS);
#else
  uint16_t pos = random16(NUM_LEDS);
#endif
  leds[pos] = ColorFromPalette(gCurrentPalette, thisindex + random8(thisdiff) / 4 , 255, currentBlending);
  thisindex = thisindex + thisinc;
}
