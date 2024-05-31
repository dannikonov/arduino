#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 8

CRGB leds[NUM_LEDS];

volatile int counter = 0;
volatile uint32_t debounce;
volatile int n = 0;

DEFINE_GRADIENT_PALETTE(heatmap_gp){
  0, 20, 232, 30,     // green 1 #14e81e
  60, 0, 234, 141,    //green 2 #00ea8d
  100, 1, 126, 213,   // blue #017ed5
  140, 181, 61, 255,  // magenta #b53dff
  195, 141, 0, 196,   // purple #8d00c4
  255, 20, 232, 30,   // green 1
};
CRGBPalette16 myPal = heatmap_gp;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);

  // button D2 - GND
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, next, CHANGE);

  // button D3 - GND
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, prev, CHANGE);
}

void next() {
  if (millis() - debounce >= 100 && digitalRead(2)) {
    debounce = millis();
    n++;
    n %= 4;
    Serial.println(n);
  }
}

void prev() {

  if (millis() - debounce >= 100 && digitalRead(3)) {
    debounce = millis();
    n--;
    n %= 4;
    Serial.println(n);
  }
}


void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); }
}

void program1() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    fadeall();

    delay(50);
  }
}

void program2() {
  static uint8_t hue = 0;
  for (int i = (NUM_LEDS)-1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    leds[i] = CRGB::Black;
    fadeall();

    delay(50);
  }
}

void program3() {
  program1();
  program2();
}

int j = 0;
void northernLights() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(myPal, (i * 4 + j) % 255);
  }
  FastLED.show();


  delay(30);
  j++;
}

typedef void (*f)();
f funcs[4] = { &program1, &program2, &program3, &northernLights };

void loop() {
  // funcs[n]();
  northernLights();
}