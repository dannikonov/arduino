#include <FastLED.h>
#include <IRremote.h>

#define NUM_LEDS 64

#define DATA_PIN 8
#define IR_RECEIVE_PIN 7

CRGB leds[NUM_LEDS];

volatile int counter = 0;
volatile uint32_t debounce;
volatile int n = 0;

DEFINE_GRADIENT_PALETTE(heatmap_northern_lights){
  0, 20, 232, 30,     // green 1 #14e81e
  60, 0, 234, 141,    //green 2 #00ea8d
  100, 1, 126, 213,   // blue #017ed5
  140, 181, 61, 255,  // magenta #b53dff
  195, 141, 0, 196,   // purple #8d00c4
  255, 20, 232, 30,   // green 1
};
CRGBPalette16 northernLigntsPal = heatmap_northern_lights;

DEFINE_GRADIENT_PALETTE(heatmap_dawn){
  0, 152, 33, 35,     // Vivid Auburn #982123
  60, 208, 63, 46,    // Persian Red #D03F2E
  140, 241, 195, 89,  // Crayola's Maize #F1C359
  195, 148, 194, 128,   // Pistachio #94C280
  255, 12, 156, 180,   // Blue-Green #0C9CB4
};
CRGBPalette16 dawnPal = heatmap_dawn;

int prevCommand = -1;

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

  IrReceiver.begin(IR_RECEIVE_PIN);
  IrReceiver.enableIRIn();
  IrReceiver.blink13(true);
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
    leds[i] = ColorFromPalette(northernLigntsPal, (i * 4 + j) % 255);
  }
  FastLED.show();


  delay(30);
  j++;
}

int j = 0;
void dawn() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(dawnPal, (i * 4 + j) % 255);
  }
  FastLED.show();


  delay(30);
  j++;
}

typedef void (*f)();
f funcs[4] = { &program1, &program2, &program3, &northernLights };

decode_results results;

void fill(CRGB color) {
  for (int i = (NUM_LEDS)-1; i >= 0; i--) {
    leds[i] = color;
    FastLED.show();
  }
}

void loop() {
  // funcs[n]();
  // northernLights();





  if (IrReceiver.decode()) {
    int command = IrReceiver.decodedIRData.command;


    Serial.println(command);

    IrReceiver.resume();

    switch (command) {
      case 22:
        fill(CRGB::DarkOrange);
        break;
      case 12:
        fill(CRGB::DarkGoldenrod);
        break;
      case 24:
        fill(CRGB::Goldenrod);
        break;
      case 94:
        fill(CRGB::DarkGoldenrod);
        break;
      case 8:
        fill(CRGB::Khaki);
        break;
      case 28:
        fill(CRGB::Maroon);
        break;
      case 90:
        fill(CRGB::MediumSlateBlue);
        break;
      case 66:
        fill(CRGB::Navy);
        break;
      case 9:
        northernLights();
        break;
      case 21:
        dawn();
        break;
    }
  }
}
