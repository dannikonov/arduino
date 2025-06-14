#include "lib_ws2812b.h"

CRGB leds[NUM_LEDS];

void init_rgb() {
  FastLED.addLeds<WS2811, RBG_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
}

void update_rgb(int color, int brightness) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(color, 255, 255);
  }

  FastLED.setBrightness(brightness);

  FastLED.show();
}