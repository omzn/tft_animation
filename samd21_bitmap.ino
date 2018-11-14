/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "Character.h"

#include "util.h"
//#include "bitmaps.h"

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 9 // Reset line for TFT (or see below...)
#define TFT_DC 8  // Data/command line for TFT

#define FPS 20

//Use this reset pin for the shield!
//#define TFT_RST  0  // you can also connect this to the Arduino reset!

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
  tft.println("Aquatan podemeter");

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
      aquatan.move(MOVE_UP);
    } else if (op == 'D') {
      aquatan.move(MOVE_DOWN);
    } else if (op == 'L') {
      aquatan.move(MOVE_LEFT);
    } else if (op == 'R') {
      aquatan.move(MOVE_RIGHT);
    }
  }
  delay(aquatan.update());
}
