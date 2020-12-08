/* File: vbe.h */
#ifndef _VBE_H_
#define _VBE_H_

#include <types.h>

#define VBE_ADDR 0x80070000
#define Pixel_Byte 3

typedef struct VBE_S {
    uint16_t ColorNumber;
    uint32_t Width;
    uint32_t Height;
    uint8_t  *VRAM;
} VBE_S;

#define Red(x)      (((x) >> 24) & 0xff)
#define Green(x)    (((x) >> 16) & 0xff)
#define Blue(x)     (((x) >>  8) & 0xff)
#define Alpha(x)    (((x) & 0xff))

extern uint32_t ScreenWidth, ScreenHeight;

void init_VBE(void);
void putPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
void sendPixel(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void setColor(uint32_t color, uint32_t background);
void putChar(uint8_t n, int32_t x, int32_t y);

#endif
