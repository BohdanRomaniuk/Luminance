#include <FastLED.h>
#include "gradients.h"

#define LED_PIN     6
#define NUM_LEDS    60
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define VOLTAGE 5
#define CURRENT_LIMIT 3000
#define MODES_AMOUNT 10
#define EFFECT_TIME 10

#define qsuba(x, b)  ((x>b)?x-b:0)

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
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
uint8_t thathue = 140;
uint8_t thatrot = 0;
int8_t thatspeed = 4;
int thatphase = 0;
uint8_t thatcutoff = 192;
uint8_t thisdiff = 1;
uint8_t thisindex = 0;
uint8_t thisfade = 224;
uint8_t thisinc = 1;
uint16_t dist = 12345;
uint8_t scale = 30;

void setup()
{
  Serial.begin(57600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTAGE, CURRENT_LIMIT);
  }

  currentPalette = RainbowColors_p;
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
      currentPalette = emeraldDragonPalette;
      noise8_pal();
      break;
    case 2:
      currentPalette = LavaColors_p;
      thisdir = 1;
      colorwaves();
      break;
    case 3:
      rainbow_beat();
      break;
    case 4:
      currentPalette = sunsetPalette;
      noise8_pal();
      break;
    case 5:
      currentPalette = landscapePalette;
      thisdir = -1;
      colorwaves();
      break;
    case 6:
      setup_two_sin(2, 1, 1, 185, 64, 1, 1, 1, 128, 192);
      two_sin();
      break;
    case 7:
      currentPalette = rgiPalette;
      noise8_pal();
      break;
    case 8:
      currentPalette = PartyColors_p;
      thisdir = 1;
      colorwaves();
      break;
    case 9:
      setup_two_sin(4, -1, 1, 0, 200, 1, 0, 0, 64, 192);
      two_sin();
      break;
    case 10:
      currentPalette = retroPalette;
      noise8_pal();
      break;
    case 11:
      currentPalette = blueCyanYellowPalette;
      thisdir = -1;
      colorwaves();
      break;
    case 12:
      setup_two_sin(4, 2, 1, 0, 128, 1, 0, 0, 0, 0);
      two_sin();
      break;
    case 13:
      currentPalette = analogousPalette;
      noise8_pal();
      break;
    case 14:
      currentPalette = RainbowColors_p;
      thisdir = 1;
      colorwaves();
      break;
    case 15:
      setup_two_sin(6, 2, 0, 24, 75, 1, 0, 1, 64, 128);
      two_sin();
      break;
    case 16:
      currentPalette = pinkPalette;
      noise8_pal();
      break;
    case 17:
      currentPalette = analogousPalette;
      thisdir = 1;
      colorwaves();
      break;
    case 18:
      setup_two_sin(2, 1, 1, 25, 152, 1, 1, 2, 64, 64);
      two_sin();
      break;
    case 19:
      currentPalette = pinksplashPalette;
      noise8_pal();
      break;
    case 20:
      currentPalette = hult65Palette;
      thisdir = -1;
      colorwaves();
      break;
    case 21:
      thisdir = 1;
      thisrot = 1;
      thisdiff = 1;

      rainbow_march();
      break;
    case 22:
      thisdir = -1;
      thisrot = 1;
      thisdiff = 1;

      rainbow_march();
      break;
    case 23:
      currentPalette = landscapePalette;
      noise8_pal();
      break;
    case 24:
      currentPalette = emeraldDragonPalette;
      thisdir = 1;
      colorwaves();
      break;
    case 25:
      currentPalette = hult65Palette;
      noise8_pal();
      break;
    case 26:
      thisinc = 1;
      thisfade = 2;
      thisdiff = 32;
      confetti_pal();
      break;
    case 27:
      currentPalette = blueCyanYellowPalette;
      noise8_pal();
      break;
    case 28:
      currentPalette = sunsetPalette;
      thisdir = -1;
      colorwaves();
      break;
    case 29:
      currentPalette = PartyColors_p;
      noise8_pal();
      break;
    case 30:
      thisdir = -1;
      thisrot = 1;
      thisdiff = 5;
      rainbow_march();
    case 31:
      currentPalette = LavaColors_p;
      noise8_pal();
      break;
    case 32:
      currentPalette = pinksplashPalette;
      thisdir = 1;
      colorwaves();
      break;
    case 33:
      currentPalette = CloudColors_p;
      noise8_pal();
      break;
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
  uint8_t startBeat = beatsin8(17, 0, 255);
  uint8_t endBeat = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (startBeat + endBeat) / 2, 8);
}

void setup_two_sin(uint8_t freq, int8_t speed1, int8_t speed2, uint8_t hue1, uint8_t hue2, int8_t dir, uint8_t rot1, uint8_t rot2, uint8_t cutoff1, uint8_t cuttoff2)
{
  allfreq = freq;
  thisspeed = speed1;
  thatspeed = speed2;
  thishue = hue1;
  thathue = hue2;
  thisdir = dir;
  thisrot = rot1;
  thatrot = rot2;
  thiscutoff = cutoff1;
  thatcutoff = cutoff1;
}

void two_sin()
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

    leds[k] = ColorFromPalette(currentPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(currentPalette, thathue, thatbright, currentBlending);
  }
}

void noise8_pal()
{
#if NUM_LEDS < 255
  uint8_t i;
#else
  uint16_t i;
#endif
  for ( i = 0; i < NUM_LEDS; i++)
  {
    uint8_t index = inoise8(i * scale, dist + i * scale) % 255;
    leds[i] = ColorFromPalette(currentPalette, index, 255, currentBlending);
  }
  dist += beatsin8(10, 1, 4) * thisdir;
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
  leds[pos] = ColorFromPalette(currentPalette, thisindex + random8(thisdiff) / 4 , 255, currentBlending);
  thisindex = thisindex + thisinc;
}

void colorwaves()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
#if NUM_LEDS < 255
  uint8_t i;
#else
  uint16_t i;
#endif

  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (i = 0 ; i < NUM_LEDS; ++i)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if (h16_128 & 0x100)
    {
      hue8 = 255 - (h16_128 >> 1);
    }
    else
    {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    index = scale8(index, 240);

    CRGB newcolor = ColorFromPalette(currentPalette, index, bri8);
    if (thisdir == 1)
    {
      nblend(leds[(NUM_LEDS - 1) - i], newcolor, 128);
    }
    else
    {
      nblend(leds[i], newcolor, 128);
    }
  }
}
