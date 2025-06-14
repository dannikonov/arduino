// DOIT ESP32 DEVKIT V1

#ifndef LIB_ST7735
#define LIB_ST7735

#define TFT_DC 12    //A0
#define TFT_CS 13    //CS
#define TFT_MOSI 14  //SDA
#define TFT_CLK 27   //SCK
#define TFT_RST 26
#define TFT_MISO 0

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

void init_tft();
void update_tft(int value_1, int value_2);
#endif