#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

byte joysticXAxisPin = A0;
byte joysticYAxisPin = A1;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


enum Direction { U = 0,
                 R = 1,
                 D = 2,
                 L = 3 };
struct Coord {
  int x;
  int y;
  Direction direction;
};


int dx, dy = 0;
int snakeSpeed = 300;
Coord head;
Coord bends[100];

int length = 1;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  head.x = SCREEN_WIDTH / 2;
  head.y = SCREEN_HEIGHT / 2;
  head.direction = R;


  // temporary
  length = 5;
}

void drawCircle(int x, int y, int radius = 2) {
  display.drawCircle(x, y, radius, WHITE);
  display.display();
}

byte centerX = 0;
byte centerY = 0;

unsigned int score = 0;

void drawBox() {
  display.drawRect(0, 0, display.width() - 1, 16, WHITE);
  display.drawRect(0, 16, display.width() - 1, 48, WHITE);
}

void drawInfo() {
  display.setCursor(2, 4);
  char str[10];
  sprintf(str, "score %d", score);
  display.print(str);
}

void loop() {
  display.clearDisplay();

  drawBox();
  drawInfo();

  scanJoystick();
  drawSnake();
  move();

  delay(10);

  delay(10);
}


void scanJoystick() {
  int x = ceil(analogRead(joysticXAxisPin) / 10);
  int y = ceil(analogRead(joysticYAxisPin) / 10);

  int homeX = 49;
  int homeY = 50;
  int threshold = 10;

  Direction newDirection = head.direction;
  if (y > homeY + threshold) {
    newDirection = U;
  }

  if (x > homeX + threshold) {
    newDirection = R;
  }

  if (y < homeY - threshold) {
    newDirection = D;
  }

  if (x < homeX - threshold) {
    newDirection = L;
  }

  if (head.direction != newDirection) {
    if (abs(head.direction - newDirection) != 2) {
      bends[0] = head;
      head.direction = newDirection;
    }
  }

  Serial.println(head.direction);
}

void drawSnake() {
  int dx, dy = 0;

  if (head.direction == U) {
    dy = 1;
  }

  if (head.direction == R) {
    dx = 1;
  }
  if (head.direction == D) {
    dy = -1;
  }

  if (head.direction == L) {
    dx = -1;
  }

  Coord tmp = head;

  for (int i = 0; i < length; i++) {
    display.drawPixel(tmp.x, tmp.y, WHITE);
    tmp.x += dx;
    tmp.y += dy;
  }

  display.display();
}

void move() {
  switch (head.direction) {
    case U:
      head.y--;
      break;
    case R:
      head.x++;
      break;
    case D:
      head.y++;
      break;
    case L:
      head.x--;
      break;
  }

  // temporary
  if (head.x < 0) {
    head.x = SCREEN_WIDTH;
  }

  if (head.x > SCREEN_WIDTH) {
    head.x = 0;
  }

  if (head.y < 0) {
    head.y = SCREEN_HEIGHT;
  }

  if (head.y > SCREEN_HEIGHT) {
    head.y = 0;
  }
}
