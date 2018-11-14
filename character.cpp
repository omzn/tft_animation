#include "Character.h"

Character::Character(Adafruit_ST7735 *display)
{
    tft = display;
    width = 32;
    height = 32;
    pos_x = 64;
    pos_y = 32;
    pattern = 0;
    frame = 0;
    speed = 5;
    orient = ORIENT_FRONT;
    status = STATUS_STOP;
    move_diff = 1;
}

void Character::start(uint8_t o)
{
    orient = o;
    status = STATUS_WAIT;
}

void Character::start(uint16_t x, uint16_t y, uint8_t o)
{
    pos_x = x; 
    pos_y = y;
    orient = o;
    status = STATUS_WAIT;
}

void Character::stop(uint8_t o)
{
    orient = o;
    status = STATUS_STOP;
}

void Character::setSpeed(uint8_t s)
{
    if (s > 0)
    {
        speed = s;
    }
}

void Character::incSpeed()
{
    speed++;
}

void Character::decSpeed()
{
    if (speed > 1)
    {
        speed--;
    }
}

void Character::setOrient(uint8_t o)
{
    orient = o;
}

void Character::move(uint8_t d, int16_t distance)
{
    status = STATUS_MOVE;
    direction = d;
    if (direction == MOVE_UP) 
    {
        target_x = pos_x;
        target_y = pos_y - distance;
    }
    else if (direction == MOVE_DOWN) 
    {
        target_x = pos_x;
        target_y = pos_y + distance;
    }
    else if (direction == MOVE_LEFT) 
    {
        target_x = pos_x - distance;
        target_y = pos_y;
    }
    else if (direction == MOVE_RIGHT) 
    {
        target_x = pos_x + distance;
        target_y = pos_y;
    }
    else if (direction == MOVE_LEFTBACK) 
    {
        target_x = pos_x - distance;
        target_y = pos_y;
    }
    else if (direction == MOVE_RIGHTBACK) 
    {
        target_x = pos_x + distance;
        target_y = pos_y;
    }
}

int Character::update()
{
    int e = 0;
    if (status != STATUS_STOP)
    {
        if (status == STATUS_MOVE)
        {
            if (direction == MOVE_UP)
            {
                tft->drawRect(pos_x,pos_y+32-move_diff,32,move_diff,ST7735_BLACK); 
                pos_y -= move_diff;
                orient = ORIENT_BACK;
            }
            else if (direction == MOVE_DOWN)
            {
                tft->drawRect(pos_x,pos_y,32,move_diff,ST7735_BLACK); 
                pos_y += move_diff;
                orient = ORIENT_FRONT;
            }
            else if (direction == MOVE_LEFT)
            {
                tft->drawRect(pos_x+32-move_diff,pos_y,move_diff,32,ST7735_BLACK); 
                pos_x -= move_diff;
                orient = ORIENT_LEFT;
            }
            else if (direction == MOVE_RIGHT)
            {
                tft->drawRect(pos_x,pos_y,move_diff,32,ST7735_BLACK); 
                pos_x += move_diff;
                orient = ORIENT_RIGHT;
            }
            else if (direction == MOVE_LEFTBACK)
            {
                tft->drawRect(pos_x+32-move_diff,pos_y,move_diff,32,ST7735_BLACK); 
                pos_x -= move_diff;
                orient = ORIENT_RIGHT;
            }
            else if (direction == MOVE_RIGHTBACK)
            {
                tft->drawRect(pos_x,pos_y,move_diff,32,ST7735_BLACK); 
                pos_x += move_diff;
                orient = ORIENT_LEFT;
            }
            e = drawBitmap16(aqua_bmp[orient][pattern], pos_x, pos_y, width, height);
            if (frame == 0)
            {
                pattern++;
                pattern %= 4;
            }
            if (pos_x == target_x && pos_y == target_y) {
                status = STATUS_WAIT;
            }
        }
        else
        {
            if (frame == 0)
            {
                e = drawBitmap16(aqua_bmp[orient][pattern++], pos_x, pos_y, width, height);
                pattern %= 4;
            }
        }
        frame++;
        frame %= speed;
    }
    int d = (1000 / FPS) - e;
    return d > 0 ? d : 0;
}

int Character::drawBitmap16(const unsigned char *data, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    int row, col, buffidx = 0;
    uint32_t startTime = millis();

    if ((x >= tft->width()) || (y >= tft->height()))
    {
        return 0;
    }
    uint16_t b = 0;
    uint16_t cnt = 0;
    for (col = 0; col < w; col++)
    { // For each scanline...
        for (row = 0; row < h; row++)
        {
            uint16_t c = pgm_read_word(data + buffidx);
            c = ((c >> 8) & 0x00ff) | ((c << 8) & 0xff00); // change back and fore
            tft->drawPixel(col + x, row + y, fixColor(c));
            buffidx += 2;
        } // end pixel
    }
    int etime = millis() - startTime;
    return etime;
}
