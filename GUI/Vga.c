/*
    File:       Vga.c

    Contains:   Provide display interface in VGA mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <vga/vga.h>

static VgaObject_t *VgaObject;
static uint32_t     ScreenX_Full;

bool_t   VGA_Toggle;
uint32_t ScreenX, ScreenY;

/* Contains: Initialize graphics mode */
int Initialize_Vga(void){

    switch (*(char*)DISPLAY_MODE_FLAG) {
        case 1:
            VGA_Toggle = false;
            return CGA_MODE;
        case 2:
            /* Get vga information */
            VgaObject->ColorNumber = *((uint16_t*)VGA_VBE_MEMORY);
            VgaObject->ScreenX     =  (uint32_t)(*((uint16_t*)(VGA_VBE_MEMORY + 2)));
            VgaObject->ScreenY     =  (uint32_t)(*((uint16_t*)(VGA_VBE_MEMORY + 4)));
            VgaObject->Vram        =  (uint8_t*)(*((uint32_t*)(VGA_VBE_MEMORY + 6)));

            ScreenX = VgaObject->ScreenX;
            ScreenY = VgaObject->ScreenY;
            ScreenX_Full = ScreenX * Pixel_Byte;

            VGA_Toggle = true;

            INITIALIZING_VIDEO_GRAPHICS_ARRAY();

            return VGA_MODE;
        default:
            return 0;
    }
}

/* Contains: Get a pixel */
uint32_t GetPixel(uint32_t x, uint32_t y){
    /* Skip the exceed's position */
    if (x >= VgaObject->ScreenX || y >= VgaObject->ScreenY) return 0;

    int pos = y * ScreenX_Full + x * Pixel_Byte;

    /* (VgaObject->Vram[pos + 2] << 24) + (VgaObject->Vram[pos + 1] << 16) + (VgaObject->Vram[pos] << 8) + 0xff */

    return READ_PIXEL(pos);
}

/* Contains: Draw a pixel */
void SetPixel(uint32_t x, uint32_t y, byte_t r, byte_t g, byte_t b, byte_t a){
    /* Skip the exceed's position */
    if (x >= VgaObject->ScreenX || y >= VgaObject->ScreenY) return;

    int pos = y * ScreenX_Full + x * Pixel_Byte;

    /* Set the alpha */
    if (a < 0xff) {
        a = a * 0x64 / 0xff;
        byte_t a_tmp = 0x64 - a;
        r = (r * a + VgaObject->Vram[pos + 2] * a_tmp) / 0x64;
        g = (g * a + VgaObject->Vram[pos + 1] * a_tmp) / 0x64;
        b = (b * a + VgaObject->Vram[pos]     * a_tmp) / 0x64;
    }

    /* Draw the pixel */
    /*
       VgaObject->Vram[pos + 0] = b;
       VgaObject->Vram[pos + 1] = g;
       VgaObject->Vram[pos + 2] = r;
    */
    PUT_PIXEL(pos, b, g, r);

    return;
}

/* Contains: Copy a pixel to other position */
void CopyPixel(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2){
    if (x1 >= VgaObject->ScreenX || y1 >= VgaObject->ScreenY) return;
    if (x2 >= VgaObject->ScreenX || y2 >= VgaObject->ScreenY) return;

    int pos1 = y1 * ScreenX_Full + x1 * Pixel_Byte, pos2 = y2 * ScreenX_Full + x2 * Pixel_Byte;

    SEND_PIXEL(pos1, pos2);

    return;
}
