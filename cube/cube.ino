#include <SPI.h>

const int LED_COUNT = 4;
const int LAYER_COUNT = 4;
unsigned int leds[LAYER_COUNT];

/**
74HC595
Q0 - Q7 - to LED
SHCP - SCK  - D13
STCP - SS   - D10
DS   - MOSI - D11
*/
const byte LATCH = 10;

/*
Layers:
0 - D4
1 - D5
2 - D6
3 - D7
*/
const unsigned int LAYERS_PINS[LAYER_COUNT] = { 4, 5, 6, 7 };

unsigned long int delayTime = 0;
unsigned int speed = 200;
unsigned int count = 0;

void initPins() {
  for (int l = 0; l < LAYER_COUNT; l++) {
    pinMode(LAYERS_PINS[l], OUTPUT);
    digitalWrite(LAYERS_PINS[l], LOW);
  }

  pinMode(LATCH, OUTPUT);
}

void setup() {
  initPins();

  Serial.begin(9600);

  for (int l = 0; l < LAYER_COUNT; l++) {
    leds[l] = 0;
  }

  SPI.begin();
  SPIWrite(0);

  delayTime = millis();
}

void flushBuffer() {
  for (int l = 0; l < LAYER_COUNT; l++) {
    SPIWrite(leds[l]);

    PORTD = B00000000;
    bitSet(PORTD, LAYERS_PINS[l]);
    delay(1);
  }
}

void clearBuffer() {
  for (int l = 0; l < LAYER_COUNT; l++) {
    leds[l] = 0;
  }
}

void drawVoxel(int l, int r, int c) {
  leds[l] |= 1 << ((r * LED_COUNT) + c);
}

// Bresenham's algorithm
void drawLine(int l, int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2; /* error value e_xy */

  for (;;) { /* loop */
    drawVoxel(l, x0, y0);
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

void drawRectangle(int l, int x0, int y0, int x1, int y1) {
  drawLine(l, x0, y0, x1, y0);
  drawLine(l, x1, y0, x1, y1);
  drawLine(l, x1, y1, x0, y1);
  drawLine(l, x0, y1, x0, y0);
}

void SPIWrite(int value) {
  digitalWrite(LATCH, LOW);
  SPI.transfer(highByte(value));
  SPI.transfer(lowByte(value));

  digitalWrite(LATCH, HIGH);
}

void rotatingLine(unsigned int layer) {
  speed = 100;
  clearBuffer();

  switch (count) {
    case 0:
    case 1:
    case 2:
    case 3:
      drawLine(layer, 0, count, LED_COUNT - 1, LED_COUNT - count - 1);
      break;
    case 4:
      drawLine(layer, 2, 0, 1, LED_COUNT - 1);
      break;
    case 5:
      drawLine(layer, 1, 0, 2, LED_COUNT - 1);
      break;
  }

  count++;
  if (count > 5) {
    count = 0;
  }
}

void heartbeat(unsigned int layer) {
  clearBuffer();

  switch (count) {
    case 0:
      drawRectangle(layer, 0, 0, LED_COUNT - 1, LED_COUNT - 1);
      break;
    case 1:
      drawRectangle(layer, 1, 1, LED_COUNT - 2, LED_COUNT - 2);
      break;
    case 2:
      drawRectangle(layer, 1, 1, LED_COUNT - 2, LED_COUNT - 2);
      break;
    case 3:
      drawRectangle(layer, 0, 0, LED_COUNT - 1, LED_COUNT - 1);
      break;
  }

  count++;
  if (count == 4) {
    count = 0;
  }
}

void randomVoxel() {
  speed = 50;

  clearBuffer();

  drawVoxel(
    random(0, LAYER_COUNT),
    random(0, LED_COUNT),
    random(0, LED_COUNT));
}

void movingRectangle(unsigned int layer) {
  speed = 100;
  clearBuffer();

  switch (count) {
    case 0: 
    drawRectangle(layer, 0, 0, 2, 2); break;
    case 1: 
    drawRectangle(layer, 1, 2, 3, 0); break;
    case 2: 
    drawRectangle(layer, 1, 1, 3, 3); break;
    case 3: 
    drawRectangle(layer, 0, 1, 2, 3); break;
  }

    count++;
  if (count == 4) {
    count = 0;
  }
}


void movingRectangle2(unsigned int layer) {
  speed = 100;
  clearBuffer();

  switch (count) {
    case 0: 
    drawRectangle(layer, 0, 0, 1, 1); break;
    case 1: 
    drawRectangle(layer, 2, 0, 3, 1); break;
    case 2: 
    drawRectangle(layer, 2, 2, 3, 3); break;
    case 3: 
    drawRectangle(layer, 0, 2, 1, 3); break;
  }

    count++;
  if (count == 4) {
    count = 0;
  }
}


unsigned int loopsCounter = 0;
void loop() {



  // heartbeat();
  if ((millis() - delayTime) > speed) {
    if (loopsCounter < 16) {
    randomVoxel();
    
    } else if (loopsCounter < 22) {
      rotatingLine(0);
    } else if (loopsCounter < 28) {
      rotatingLine(1);
    } else if (loopsCounter < 34) {
      rotatingLine(2);
    } else if (loopsCounter < 40) {
      rotatingLine(3);
    
    } else if (loopsCounter < 44) {
      movingRectangle(0);
    } else if (loopsCounter < 48) {
      movingRectangle(1);
    } else if (loopsCounter < 52) {
      movingRectangle(2);  
    } else if (loopsCounter < 56) {
      movingRectangle(3);
    

    } else if (loopsCounter < 60) {
      movingRectangle2(0);
    } else if (loopsCounter < 64) {
      movingRectangle2(1);
    } else if (loopsCounter < 68) {
      movingRectangle2(2);  
    } else if (loopsCounter < 72) {
      movingRectangle2(3);

    } else if (loopsCounter < 76) {
      heartbeat(0);
    } else if (loopsCounter < 80) {
      heartbeat(1);
    } else if (loopsCounter < 84) {
      heartbeat(2);  
    } else if (loopsCounter < 88) {
      heartbeat(3);
    } else {
      clearBuffer();
    }

    loopsCounter++;

    if (loopsCounter == 97) {
      loopsCounter = 0;
    }

    delayTime = millis();
  }


  flushBuffer();
  
}