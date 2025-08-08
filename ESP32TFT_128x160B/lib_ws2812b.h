#ifndef LIB_WS2812B
#define LIB_WS2812B

#include "FastLED.h"

#define NUM_LEDS 64
#define RBG_PIN 4


void init_rgb();
void update_brightness(int brightness);
void update_rgb(int color);



void off();
void manualColor();
void northernLights();
void dawn();


typedef void (*fn)();


#endif