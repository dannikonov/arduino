#include <SPI.h>

/**
74HC595
Q0 - Q7 - to LED
SHCP - SCK  - D13
STCP - SS   - D10
DS   - MOSI - D11

*/
const byte LATCH = 10;

void setup() {
  SPI.begin();

  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
  SPI.transfer(0);
  digitalWrite(LATCH, HIGH);
}


void loop() {
  for (int i = 0; i < 2 ^ 8; i++) {
    digitalWrite(LATCH, LOW);
    SPI.transfer(i);
    digitalWrite(LATCH, HIGH);
    delay(200);
  }
}