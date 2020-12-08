/*
    File:       Graphics.c

    Contains:   Provide graphics interface in vga mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/macro.h>
#include <lib/math.h>
#include <vga/color.h>
#include <vga/graphics.h>
#include <vga/vga.h>

#include "Resources/DotFonts.h"

/* Contains: Point lattice asscii (width = 8px, height = 16px) */
void VG_AsciiChar(int x, int y, char n, uint32_t color, uint32_t background){
    int set_x, set_y, rows, cols, subs_ascii, subs_bit;

    byte_t red_b = Red(background), green_b = Green(background), blue_b = Blue(background), alpha_b = Alpha(background);

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);

    rows = x + 8, cols = y + 16, subs_ascii = 0;

    if (n < 32 || n > 126) n = 127 - 32;
    else n -= 32;

    for (set_y = y; set_y < cols; set_y++, subs_ascii++) {
        for (set_x = x, subs_bit = -1; set_x < rows; set_x++, subs_bit++) {
            /* Draw background */
            if (Bitof(FontAscii[n][subs_ascii], 8 - subs_bit) == 0) {
                if (background == COLOR_TRANSPARENT) continue;
                SetPixel(set_x, set_y, red_b, green_b, blue_b, alpha_b);
            } else {
                /* Draw a piexl */
                if (color == COLOR_TRANSPARENT) continue;
                SetPixel(set_x, set_y, red, green, blue, alpha);
            }
        }
    }

    return;
}

/* Contains: Point lattice string */
void VG_AsciiString(int x, int y, cstring_t string, uint32_t color, uint32_t background){
    int set_x = x, set_y = y, line_length = 0;
    while (*string != '\0'){
        /* Ascii char: ' ' */
        if (*string < 32) {
            switch (*string) {
                case '\n': {
                    set_x = x;
                    set_y += 16;
                    line_length = 0;
                    break;
                }
                case '\t': {
                    set_x += (4 - (line_length % 4)) * 8;
                    line_length += 4 - (line_length % 4);
                    break;
                }
                case '\b': {
                    set_x -= 8;
                    line_length--;
                    break;
                }
                case '\r': {
                    set_x = x;
                    line_length = 0;
                    break;
                }
                default: goto GO_ON;
            }
            goto END;
        }
    GO_ON:
        VG_AsciiChar(set_x, set_y, *string, color, background);
        set_x += 8;
        line_length++;
    END:
        string++;
    }

    return;
}

/* Contains: Point a line */
void VG_Line(int x1, int y1, int x2, int y2, uint32_t color){
    int dx, dy, x, y, p, k;

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    k  = (x2 - x1) * (y2 - y1);

    if (dx >= dy) {
        p = (dy << 1) - dx;
        if (x1 < x2) {
            x = x1;
            y = y1;
        } else {
            y = y2;
            x = x2;
            x2 = x1;
        }

        while (x < x2) {
            SetPixel(x, y, red, green, blue, alpha);
            ++x;

            if (p < 0) {
                p += dy << 1;
            } else {
                if (k > 0) ++y;
                else --y;

                p += (dy - dx) << 1;
            }
        }
    } else {
        p = (dx << 1) - dy;
        if (y1 < y2) {
            x = x1;
            y = y1;
        }
        else {
            y = y2;
            x = x2;
            y2 = y1;
        }

        while (y < y2) {
            SetPixel(x, y, red, green, blue, alpha);
            ++y;

            if (p < 0) {
                p += dx << 1;
            } else {
                if (k > 0) ++x;
                else --x;

                p += (dx - dy) << 1;
            }
        }
    }

    return;
}

/* Contains: Draw a rectangle */
void VG_Rect(int x, int y, int width, int height, uint32_t color){
    int set_x, set_y;

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);

    for (set_y = y + height - 1; set_y >= y; set_y--) {
        for (set_x = x + width - 1; set_x >= x; set_x--)
            SetPixel(set_x, set_y, red, green, blue, alpha);
    }

    return;
}

/* Contains: Draw a frame */
void VG_Frame(int x, int y, int width, int height, uint32_t color){
    int set_x, set_y, y_tmp = y + height;

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);
    
    for (set_x = x + width - 1; set_x >= x; set_x--) {
        SetPixel(set_x,     y, red, green, blue, alpha);
        SetPixel(set_x, y_tmp, red, green, blue, alpha);
    }
    
    for (set_y = y + height - 1, width += x - 1; set_y > y; set_y--) {
        SetPixel(x,     set_y, red, green, blue, alpha);
        SetPixel(width, set_y, red, green, blue, alpha);
    }

    return;
}

/* Contains: Draw a circle */
void VG_Circle(int x, int y, int radius, uint32_t color){
    int set_x, set_y, range_x = x + radius - 1, range_y = radius - 1, x_tmp = (x << 1) - 1, y_tmp = (y << 1) - 1;

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);
    
    for (set_x = x; set_x <= range_x; set_x++) {
        for (; radius * radius < range_y * range_y + (set_x - x)*(set_x - x); range_y--);
        for (set_y = range_y + y; set_y >= y; set_y--){
            SetPixel(x_tmp - set_x, y_tmp - set_y, red, green, blue, alpha);
            SetPixel(        set_x, y_tmp - set_y, red, green, blue, alpha);
            SetPixel(x_tmp - set_x,         set_y, red, green, blue, alpha);
            SetPixel(        set_x,         set_y, red, green, blue, alpha);
        }
    }

    return;
}

/* Contains: Draw a ring */
void VG_Ring(int x, int y, int radius, uint32_t color){
    int set_x = 0, set_y = radius, p = 1 - radius;

    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);

    while (set_x < set_y) {
        SetPixel(x + set_x, y + set_y, red, green, blue, alpha);
        SetPixel(x - set_x, y + set_y, red, green, blue, alpha);
        SetPixel(x - set_x, y - set_y, red, green, blue, alpha);
        SetPixel(x + set_x, y - set_y, red, green, blue, alpha);
        SetPixel(x + set_y, y + set_x, red, green, blue, alpha);
        SetPixel(x - set_y, y + set_x, red, green, blue, alpha);
        SetPixel(x - set_y, y - set_x, red, green, blue, alpha);
        SetPixel(x + set_y, y - set_x, red, green, blue, alpha);

        set_x++;
        if (p < 0) p += (set_x << 1) + 1;
        else {
            set_y--;
            p += ((set_x - set_y) << 1) + 1;
        }
    }

    return;
}

/* Contains: Draw a Rounded Rectangle */
void VG_RoundedRect(int x, int y, int width, int height, int radius, uint32_t color){
    byte_t red   = Red(color);
    byte_t green = Green(color);
    byte_t blue  = Blue(color);
    byte_t alpha = Alpha(color);

    return;
}
