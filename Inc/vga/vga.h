/*
    File:       vga.h

    Contains:   Provide display interface in VGA mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_VGA__
#define __VGA_VGA__

#include <sys/types.h>

#define DISPLAY_MODE_FLAG 0x80005000
#define VGA_VBE_MEMORY    0x80070000
#define Pixel_Byte        3

#define CGA_MODE          1
#define VGA_MODE          2

typedef struct VgaObject_t {
    uint16_t ColorNumber;
    uint32_t ScreenX;
    uint32_t ScreenY;
    uint8_t  *Vram;
} VgaObject_t;

extern bool_t VGA_Toggle;
extern uint32_t ScreenX, ScreenY;

extern void     INITIALIZING_VIDEO_GRAPHICS_ARRAY();
extern uint32_t READ_PIXEL(int position);
extern void     PUT_PIXEL(int position, byte_t bule, byte_t green, byte_t red);
extern void     SEND_PIXEL(int position1, int position2);

int      Initialize_Vga();
uint32_t GetPixel(uint32_t x, uint32_t y);
void     SetPixel(uint32_t x, uint32_t y, byte_t r, byte_t g, byte_t b, byte_t a);
void     CopyPixel(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

#endif
