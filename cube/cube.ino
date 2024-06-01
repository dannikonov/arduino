#include <SPI.h>

/**
74HC595
Q0 - Q7 - to LED
SHCP - SCK  - D13
STCP - SS   - D10
DS   - MOSI - D11

*/
const byte LATCH = 10;



const int LED_COUNT = 4;
bool leds[LED_COUNT][LED_COUNT];

void setup() {
  Serial.begin(9600);
  SPI.begin();

  for (int r = 0; r < LED_COUNT; r++) {
    for (int c = 0; c < LED_COUNT; c++) {
      leds[r][c] = 0;
    }
  }

  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
  SPI.transfer(0);
  digitalWrite(LATCH, HIGH);
}
void flushBuffer() {
  int value = 0;
  for (int r = LED_COUNT - 1; r >= 0; r--) {
    for (int c = LED_COUNT - 1; c >= 0; c--) {
      value |= leds[r][c];

      if (r != 0 || c != 0) {
        value = value << 1;
      }
    }
  }

  Serial.println(value);
  SPIWrite(value);
}

void clearBuffer() {
  for (int r = LED_COUNT - 1; r >= 0; r--) {
    for (int c = LED_COUNT - 1; c >= 0; c--) {
      leds[r][c] = 0;
    }
  }
}

void draw_point(int r, int c, bool state) {
  leds[r][c] = 1;
}

// Bresenham's algorithm
void draw_line(int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2; /* error value e_xy */

  for (;;) { /* loop */
    draw_point(x0, y0, 1);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    } /* e_xy+e_x > 0 */
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    } /* e_xy+e_y < 0 */
  }
}

void draw_rectangle(int x0, int y0, int x1, int y1) {
  draw_line(x0, y0, x1, y0);
  draw_line(x1, y0, x1, y1);
  draw_line(x1, y1, x0, y1);
  draw_line(x0, y1, x0, y0);
}

void SPIWrite(int value) {
  digitalWrite(LATCH, LOW);
  SPI.transfer(highByte(value));
  SPI.transfer(lowByte(value));

  digitalWrite(LATCH, HIGH);
}

void rotating_line() {
  for (int r = 0; r < 2; r++) {
    for (int i = 0; i < LED_COUNT; i++) {
      draw_line(0, i, LED_COUNT - 1, LED_COUNT - i - 1);
      flushBuffer();
      clearBuffer();
      delay(200);
    }

    for (int i = LED_COUNT - 2; i > 0; i--) {
      draw_line(i, 0, LED_COUNT - i - 1, LED_COUNT - 1);
      flushBuffer();
      clearBuffer();
      delay(200);
    }
  }
}

void heartbeat() {
  for (int i = 0; i < 4; i++) {
    draw_rectangle(0, 0, LED_COUNT - 1, LED_COUNT - 1);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(1, 1, LED_COUNT - 2, LED_COUNT - 2);
    flushBuffer();
    clearBuffer();
    delay(300);
  }
}

void random_pixel() {
  for (int i = 0; i < 80; i++) {
    int r = random(0, LED_COUNT);
    int c = random(0, LED_COUNT);

    draw_point(r, c, 1);
    flushBuffer();
    clearBuffer();
    delay(40);
  }
}

void moving_rectangle() {
  for (int i = 0; i < 3; i++) {
    draw_rectangle(0, 0, 2, 2);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(1, 2, 3, 0);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(1, 1, 3, 3);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(0, 1, 2, 3);
    flushBuffer();
    clearBuffer();
    delay(300);
  }
}

void moving_rectangle2() {
  for (int i = 0; i < 4; i++) {
    draw_rectangle(0, 0, 1, 1);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(2, 0, 3, 1);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(2, 2, 3, 3);
    flushBuffer();
    clearBuffer();
    delay(300);
    draw_rectangle(0, 2, 1, 3);
    flushBuffer();
    clearBuffer();
    delay(300);
  }
}

void loop() {
  rotating_line();
  heartbeat();
  random_pixel();
  moving_rectangle2();
}