#include <SPI.h>
#include "Character.h"

#include "util.h"

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 9 // Reset line for TFT (or see below...)
#define TFT_DC 8  // Data/command line for TFT

#define FPS 20

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Character aquatan = Character(&tft);

void setup(void)
{
  Serial.begin(115200);

  tft.initR(INITR_MINI160x80);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(fixColor(ST7735_WHITE));
  tft.println("Aquatan animation SAMD21");

  delay(1000);
  aquatan.start(80-16,32,ORIENT_FRONT);
}

void loop()
{
  if (Serial.available() > 0)
  {
    // 受信したデータの1バイトを読み取る
    byte op = Serial.read();
    if (op == 'f') {
      aquatan.setOrient(ORIENT_FRONT);
    } else if (op == 'l') {
      aquatan.setOrient(ORIENT_LEFT);
    } else if (op == 'r') {
      aquatan.setOrient(ORIENT_RIGHT);
    } else if (op == 'b') {
      aquatan.setOrient(ORIENT_BACK);
    } else if (op == '-') {
      aquatan.incSpeed();
    } else if (op == '+') {
      aquatan.decSpeed();
    } else if (op == 'U') {
      aquatan.move(MOVE_UP,32);
    } else if (op == 'D') {
      aquatan.move(MOVE_DOWN,32);
    } else if (op == 'L') {
      aquatan.move(MOVE_LEFT,32);
    } else if (op == 'R') {
      aquatan.move(MOVE_RIGHT,32);
    } else if (op == 'M') {
      aquatan.move(MOVE_LEFTBACK,32);
    } else if (op == 'N') {
      aquatan.move(MOVE_RIGHTBACK,32);
    }
  }
  delay(aquatan.update());
}
