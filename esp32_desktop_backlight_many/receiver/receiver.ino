// esp 32 devkit v1

// https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

/*
screen
*/
#define TFT_DC 12    //A0
#define TFT_CS 13    //CS
#define TFT_MOSI 14  //SDA
#define TFT_CLK 27   //SCK
#define TFT_RST 26
#define TFT_MISO 0

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

/* encoders */
#include <ESP32Encoder.h>
ESP32Encoder encoder_1;
ESP32Encoder encoder_2;
#define ENCODER_1_A 22
#define ENCODER_1_B 23
#define MODE_BUTTON 1

#define ENCODER_2_A 19
#define ENCODER_2_B 21
#define DEVICE_BUTTON 3




#include <esp_now.h>
#include <WiFi.h>
#include "settings.h"

#define LED_PIN 2

typedef struct struct_message {
  int lux;
} struct_message;

struct_message myData;
int lux = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

int i = 0;
int delayTime = 1;
uint task2delayTime = 30;

#define NUM_LEDS 96
#include "FastLED.h"
#define RBG_PIN 19
CRGB leds[NUM_LEDS];
byte counter;

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



void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);

  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("lux: ");
  Serial.println(myData.lux);
  Serial.println();
  lux = myData.lux;

  Task2code();
}


void test() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(counter + i * 2, 255, 255);
  }

  FastLED.show();
}

int speed = 60;

void off() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Black;
  }1

  FastLED.show();
}

void blue() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Blue;
  }

  FastLED.show();
}

void northernLights() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = ColorFromPalette(northernLigntsPal, (j * 4 + i / speed) % 255);
  }

  FastLED.show();
}


void dawn() {
  for (uint8_t j = 0; j < NUM_LEDS; j++) {
    leds[j] = ColorFromPalette(dawnPal, (j * 4 + i / speed) % 255);
  }
  FastLED.show();
}

void Task1code(void* pvParameters) {
}


int currentProgram;
unsigned long lastSwitch;

// void Task2code(void* pvParameters) {
void Task2code() {
  Serial.print("Task2 running on core ");
  // Serial.println(xPortGetCoreID());

  

    if (i % task2delayTime % 100 == 0) {
      // tft.fillScreen(ST77XX_BLACK);
      // tft.setCursor(0, 0);
      // tft.setTextColor(ST77XX_WHITE);
      // tft.setTextWrap(true); // ??

      char buffer[8];
      sprintf(buffer, "avt: %d", lux);

      tft.print(buffer);
    }

    if (lastSwitch < millis() - 5000) {
      switchProgram(lux);
    }

    delay(task2delayTime);
    i += task2delayTime;

}

void switchProgram(int value) {
  if (value > 500) {
    off();
    if (currentProgram != 0) {
      currentProgram = 0;
      lastSwitch = millis();
    }
  } else if (value > 450) {
    blue();
    if (currentProgram != 1) {
      currentProgram = 1;
      lastSwitch = millis();
    }
  } else if (value > 200) {
    dawn();
    if (currentProgram != 2) {
      currentProgram = 2;
      lastSwitch = millis();
    }
  } else {
    northernLights();
    if (currentProgram != 3) {
      currentProgram = 3;
      lastSwitch = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);



  // screen
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);

  // encoders
  encoder_1.attachHalfQuad(ENCODER_1_A, ENCODER_1_B);
  encoder_2.attachHalfQuad(ENCODER_2_A, ENCODER_2_B);


  // blink led
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Started");






  // if (!WiFi.config(ip, gateway, subnet, dns, dns)) {
  //   Serial.println("Failed to configure");
  // }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));


  // xTaskCreatePinnedToCore(
  //   Task1code, /* Task function. */
  //   "Task1",   /* name of task. */
  //   10000,     /* Stack size of task */
  //   NULL,      /* parameter of the task */
  //   1,         /* priority of the task */
  //   &Task1,    /* Task handle to keep track of created task */
  //   0);        /* pin task to core 0 */
  // delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  // xTaskCreatePinnedToCore(
  //   Task2code, /* Task function. */
  //   "Task2",   /* name of task. */
  //   10000,     /* Stack size of task */
  //   NULL,      /* parameter of the task */
  //   1,         /* priority of the task */
  //   &Task2,    /* Task handle to keep track of created task */
  //   1);        /* pin task to core 1 */
  // delay(500);


  FastLED.addLeds<WS2811, RBG_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
}

void loop() {
}


// https://wiki.amperka.ru/products:esp32-wroom-wifi-devkit-v1
