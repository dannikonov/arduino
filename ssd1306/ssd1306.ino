#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

byte joysticXAxisPin = A0;
byte joysticYAxisPin = A1;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int dx, dy = 0;

byte coef = 150;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void drawCircle(int x, int y, int radius = 2) {
  display.drawCircle(x, y, radius, WHITE);
  display.display();
}

byte centerX = 0;
byte centerY = 0;

void loop() {
  dx = (500 - analogRead(joysticXAxisPin)) / coef;
  dy = (500 - analogRead(joysticYAxisPin)) / coef;

  centerX = (centerX - dx) % SCREEN_WIDTH;
  centerY = (centerY + dy) % SCREEN_HEIGHT;  

  drawCircle(centerX, centerY);
  display.clearDisplay();
  delay(10);
}
