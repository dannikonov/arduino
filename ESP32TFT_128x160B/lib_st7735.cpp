#include "lib_st7735.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

void init_tft() {
  tft.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setTextSize(0);
}


void update_tft(int value_1, int value_2) {
  // tft.setCursor(0, 0);
  // tft.println("                 ");
  // tft.println("                 ");

tft.setCursor(0, 0);
  tft.fillRect(0, 0, 18, 16, ST77XX_BLACK);

  
  tft.println(value_1);
  tft.println(value_2);
}
