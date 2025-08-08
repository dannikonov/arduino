// ESP32-WROOM

// #include "lib_st7735.h"
#include "lib_ssd1306.h"
#include "lib_ws2812b.h"


fn functions[] = { off, manualColor, northernLights, dawn };
float p = 3.1415926;

uint currentProgram = 0;
int delayTime = 30;


#include <ESP32Encoder.h>

void IRAM_ATTR colorEncoderChanged(void* arg) {
  ESP32Encoder* enc = (ESP32Encoder*)arg;
  Serial.println(enc->getCount());
  if (enc->getCount() > 255) {
    enc->setCount(0);
  }

  if (enc->getCount() < 0) {
    enc->setCount(255);
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


#define ENCODER_1_A 15
#define ENCODER_1_B 23
#define MODE_BUTTON 34

#define ENCODER_2_A 19
#define ENCODER_2_B 18
#define DEVICE_BUTTON 35

#define BTN1 26
#define BTN2 25
#define BTN3 33
#define BTN4 32


int btnTimerMode = 0;
void IRAM_ATTR modeBtnClick() {
  if (millis() - btnTimerMode > 200) {
    btnTimerMode = millis();
    Serial.println("mode btn");
    // encoder_2.setCount(0);
    currentProgram++;
    currentProgram %= 4;

    Serial.print(currentProgram);
  }
}

int btnTimerDevice = 0;
void IRAM_ATTR deviceBtnClick() {
  if (millis() - btnTimerDevice > 200) {
    btnTimerDevice = millis();
    Serial.println("device btn");
    // encoder_1.setCount(0);
    currentProgram--;
    currentProgram %= 4;

    Serial.print(currentProgram);
  }
}

int btn1Timer = 0;
void IRAM_ATTR btn1Click() {
  if (millis() - btn1Timer > 200) {
    btnTimerDevice = millis();
    Serial.println("btn 1 clicked");
  }
}

int btn2Timer = 0;
void IRAM_ATTR btn2Click() {
  if (millis() - btn2Timer > 200) {
    btnTimerDevice = millis();
    Serial.println("btn 2 clicked");
  }
}

int btn3Timer = 0;
void IRAM_ATTR btn3Click() {
  if (millis() - btn3Timer > 200) {
    btnTimerDevice = millis();
    Serial.println("btn 3 clicked");
  }
}

int btn4Timer = 0;
void IRAM_ATTR btn4Click() {
  if (millis() - btn4Timer > 200) {
    btnTimerDevice = millis();
    Serial.println("btn 4 clicked");
  }
}


void setup(void) {

  pinMode(MODE_BUTTON, INPUT_PULLDOWN);
  pinMode(DEVICE_BUTTON, INPUT_PULLDOWN);

  attachInterrupt(MODE_BUTTON, modeBtnClick, RISING);
  attachInterrupt(DEVICE_BUTTON, deviceBtnClick, RISING);

  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);

  attachInterrupt(BTN1, btn1Click, FALLING);
  attachInterrupt(BTN2, btn2Click, FALLING);
  attachInterrupt(BTN3, btn3Click, FALLING);
  attachInterrupt(BTN4, btn4Click, FALLING);




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



  // init_tft();
  init_display();
  init_rgb();
}


byte counter;

int prev_encoder_1, prev_encoder_2;
void loop() {
  bool changed = false;
  if (currentProgram == 1) {
    if (prev_encoder_1 != encoder_1.getCount()) {
      changed = true;
      prev_encoder_1 = encoder_1.getCount();

      update_rgb(prev_encoder_1);
    }
  }


  if (prev_encoder_2 != encoder_2.getCount()) {
    changed = true;
    prev_encoder_2 = encoder_2.getCount();
    update_brightness(prev_encoder_2);
  }

  if (changed) {
    update_display(prev_encoder_1, prev_encoder_2);
  }

  functions[currentProgram]();
  delay(delayTime);
}

// https://forum.arduino.cc/t/i-dont-quite-understand-how-tft-st7735-displays-work/1244639/8
// https://forums.adafruit.com/viewtopic.php?t=183237
