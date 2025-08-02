#include "lib_ws2812b.h"

CRGB leds[NUM_LEDS];

int speed = 60;

DEFINE_GRADIENT_PALETTE(heatmap_northern_lights){
  0, 20, 232, 30,     // green 1 #14e81e
  60, 0, 234, 141,    // green 2 #00ea8d
  100, 1, 126, 213,   // blue #017ed5
  140, 181, 61, 255,  // magenta #b53dff
  195, 141, 0, 196,   // purple #8d00c4
  255, 20, 232, 30,   // green 1
};
CRGBPalette16 northernLigntsPal = heatmap_northern_lights;

DEFINE_GRADIENT_PALETTE(heatmap_dawn){
  0, 152, 33, 35,      // Vivid Auburn #982123
  60, 208, 63, 46,     // Persian Red #D03F2E
  140, 241, 195, 89,   // Crayola's Maize #F1C359
  195, 148, 194, 128,  // Pistachio #94C280
  255, 12, 156, 180,   // Blue-Green #0C9CB4
};
CRGBPalette16 dawnPal = heatmap_dawn;

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


void off() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Black;
  }

  FastLED.show(); 
}

void manualColor() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Blue;
  }

  FastLED.show();
}

void northernLights() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = ColorFromPalette(northernLigntsPal, (j * 4 + millis()/ speed) % 255);
  }

  FastLED.show();
}


void dawn() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = ColorFromPalette(dawnPal, (j * 4 + millis()/ speed) % 255);
  }
  FastLED.show();
}