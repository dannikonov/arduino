#include "lib_st7735.h"
#include "lib_ws2812b.h"

// extern leds;


// These pins will also work for the 1.8" TFT shield

//ESP32-WROOM














float p = 3.1415926;

#include <ESP32Encoder.h>

void IRAM_ATTR colorEncoderChanged(void* arg) {
  ESP32Encoder* enc = (ESP32Encoder*)arg;
  Serial.println(enc->getCount());
  if (enc->getCount() > 256) {
    enc->setCount(0);
  }

  Serial.print(enc->getCount());
}

void IRAM_ATTR brightnessEncoderChanged(void* arg) {

  ESP32Encoder* enc = (ESP32Encoder*)arg;
  Serial.println(enc->getCount());
  if (enc->getCount() > 100) {
    enc->setCount(100);
  }

  if (enc->getCount() < 0) {
    enc->setCount(0);
  }

  Serial.print(enc->getCount());
}


ESP32Encoder encoder_1(true, colorEncoderChanged);
ESP32Encoder encoder_2(true, brightnessEncoderChanged);
#define ENCODER_1_A 22
#define ENCODER_1_B 23
#define MODE_BUTTON 34

#define ENCODER_2_A 19
#define ENCODER_2_B 21
#define DEVICE_BUTTON 35


int btnTimerMode = 0;
void IRAM_ATTR modeBtnClick() {
  if (millis() - btnTimerMode > 100) {
    btnTimerMode = millis();
    Serial.println("mode btn");
    encoder_2.setCount(0);
  }
}

int btnTimerDevice = 0;
void IRAM_ATTR deviceBtnClick() {
  if (millis() - btnTimerDevice > 100) {
    btnTimerDevice = millis();
    Serial.println("device btn");
    encoder_1.setCount(0);
  }
}

void setup(void) {

  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(DEVICE_BUTTON, INPUT_PULLUP);

  attachInterrupt(MODE_BUTTON, modeBtnClick, RISING);
  attachInterrupt(DEVICE_BUTTON, deviceBtnClick, RISING);



  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  // Use this initializer if using a 1.8" TFT screen:
  


  Serial.println(F("Initialized"));

  uint16_t time = millis();
  
  time = millis() - time;



  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder_1.attachSingleEdge(ENCODER_1_A, ENCODER_1_B);
  encoder_1.clearCount();
  encoder_2.attachSingleEdge(ENCODER_2_A, ENCODER_2_B);


  // encoder_2.setFilter(100);
  encoder_2.setCount(50);



  init_tft();
  init_rgb();
}


byte counter;

int prev_encoder_1, prev_encoder_2;
void loop() {
  if (prev_encoder_1 != encoder_1.getCount() || prev_encoder_2 != encoder_2.getCount()) {
    prev_encoder_1 = encoder_1.getCount();
    prev_encoder_2 = encoder_2.getCount();

    update_rgb(prev_encoder_1, prev_encoder_2);
    update_tft(prev_encoder_1, prev_encoder_2);
  }

  delay(50);
}

// https://forum.arduino.cc/t/i-dont-quite-understand-how-tft-st7735-displays-work/1244639/8
// https://forums.adafruit.com/viewtopic.php?t=183237
