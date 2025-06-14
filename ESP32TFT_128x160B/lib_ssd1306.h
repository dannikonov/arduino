#ifndef LIB_SSD1306
#define LIB_SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

void init_display();
void update_display(int value_1, int value_2);

#endif