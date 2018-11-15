#ifndef CHARACTER_H
#define CHARACTER_H

#include "Arduino.h"
#include "bitmaps.h"
#include "util.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// 20 frames per second
#define FPS 20

enum { ORIENT_FRONT = 0, ORIENT_LEFT, ORIENT_RIGHT, ORIENT_BACK };
enum { STATUS_WAIT = 0, STATUS_STOP, STATUS_MOVE };
enum {
  MOVE_UP = 0,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_DOWN,
  MOVE_LEFTBACK,
  MOVE_RIGHTBACK
};

class Character {
public:
  Character(Adafruit_ST7735 *display);
  void start(uint8_t o);
  void start(uint16_t x, uint16_t y, uint8_t o);
  void stop(uint8_t o);
  void setSpeed(uint8_t s);
  void incSpeed();
  void decSpeed();
  void setMoveDiffBySpeed();
  void setOrient(uint8_t o);
  void move(uint8_t d, int16_t dist);
  int update(); // returns millisecs to wait
protected:
  Adafruit_ST7735 *tft;
  int16_t pos_x, pos_y, width, height, target_x, target_y;
  uint8_t status;
  uint8_t speed;   // speed (1-) drawing interval (5 is slow, 1 is fast)
  uint8_t pattern; // character pattern (0-4)
  uint8_t frame;
  uint8_t orient;
  uint8_t direction;
  uint8_t move_diff;
  int drawBitmap16(const unsigned char *data, uint8_t x, uint8_t y, uint8_t w,
                   uint8_t h);
};

#endif
