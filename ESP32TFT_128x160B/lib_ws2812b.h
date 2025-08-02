#ifndef LIB_WS2812B
#define LIB_WS2812B

#include "FastLED.h"

#define NUM_LEDS 96
#define RBG_PIN 33


void init_rgb();
void update_rgb(int color, int brightness);



void off();
void manualColor();
void northernLights();
void dawn();


typedef void (*fn)();


#endif