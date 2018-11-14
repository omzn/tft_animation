#ifndef UTIL_H
#define UTIL_H

#define Serial SerialUSB

/* 
    RGB565 -> BGR565 conversion
*/
inline uint16_t fixColor(uint16_t color)
{
    return (color & 0xf800) >> 11 | (color & 0x07e0) | (color & 0x001f) << 11;
}

#endif