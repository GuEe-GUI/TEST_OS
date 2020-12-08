/* File: vbe.c */
#include <vbe.h>

static VBE_S *VBE;
static uint32_t ScreenLength;

uint32_t ScreenWidth, ScreenHeight;
static uint8_t colors[6] = {0};

#include "fonts.h"

void init_VBE(void) {
    VBE->ColorNumber = *((uint16_t*)VBE_ADDR);
    VBE->Width       =  (uint32_t)(*((uint16_t*)(VBE_ADDR + 2)));
    VBE->Height      =  (uint32_t)(*((uint16_t*)(VBE_ADDR + 4)));
    VBE->VRAM        =  (uint8_t*)(*((uint32_t*)(VBE_ADDR + 6)));
    ScreenWidth  = VBE->Width;
    ScreenHeight = VBE->Height;
    ScreenLength = ScreenWidth * Pixel_Byte;
    return;
}

void putPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= VBE->Width || y >= VBE->Height) return;

    int pos = y * ScreenLength + x * Pixel_Byte;
    VBE->VRAM[pos + 0] = b;
    VBE->VRAM[pos + 1] = g;
    VBE->VRAM[pos + 2] = r;
    return;
}

void sendPixel(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    if (x1 >= VBE->Width || y1 >= VBE->Height) return;
    if (x2 >= VBE->Width || y2 >= VBE->Height) return;

    int pos1 = y1 * ScreenLength + x1 * Pixel_Byte;
    int pos2 = y2 * ScreenLength + x2 * Pixel_Byte;
    VBE->VRAM[pos2 + 0] = VBE->VRAM[pos1 + 0];
    VBE->VRAM[pos2 + 1] = VBE->VRAM[pos1 + 1];
    VBE->VRAM[pos2 + 2] = VBE->VRAM[pos1 + 2];
}

void setColor(uint32_t color, uint32_t background) {
    colors[0] = Red(color);
    colors[1] = Green(color);
    colors[2] = Blue(color);
    colors[3] = Red(background);
    colors[4] = Green(background);
    colors[5] = Blue(background);

    return;
}

void putChar(uint8_t n, int32_t x, int32_t y) {
    int set_x, set_y, rows, cols, subs_ascii, subs_bit;

    rows = x + 8, cols = y + 16, subs_ascii = 0;

    if (n < 32 || n > 126) n = 95;
    else n -= 32;

    for (set_y = y; set_y < cols; ++set_y, ++subs_ascii) {
        for (set_x = x, subs_bit = 7; set_x < rows; ++set_x, --subs_bit) {
            if ((AsciiFonts[n][subs_ascii] >> subs_bit & 1)) {
                putPixel(set_x, set_y, colors[0], colors[1], colors[2]);
            } else {
                putPixel(set_x, set_y, colors[3], colors[4], colors[5]);
            }
        }
    }

    return;
}
