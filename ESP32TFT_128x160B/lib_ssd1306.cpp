#include "lib_ssd1306.h"


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void init_display() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Address 0x3D for 128x64
    // Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

display.print("sdfljasdlkfjfds");
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  // display.clearDisplay();  
display.setRotation(1);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  
}


void update_display(int value_1, int value_2) {
  display.clearDisplay();

  display.setCursor(16, 0);
  display.print(value_1);
  display.setCursor(16, 10);
  display.print(value_2);
  display.display();

  
}
