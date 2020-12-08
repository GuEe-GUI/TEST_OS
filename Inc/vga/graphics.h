/*
    File:       graphics.h

    Contains:   Provide graphics interface in vga mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_GRAPHICS__
#define __VGA_GRAPHICS__

#include <sys/types.h>

/* VG --> VGA Graphics */

void VG_AsciiChar(int x, int y, char n, uint32_t color, uint32_t background);
void VG_AsciiString(int x, int y, cstring_t string, uint32_t color, uint32_t background);
void VG_Line(int x1, int y1, int x2, int y2, uint32_t color);
void VG_Rect(int x, int y, int width, int height, uint32_t color);
void VG_Frame(int x, int y, int width, int height, uint32_t color);
void VG_Circle(int x, int y, int radius, uint32_t color);
void VG_Ring(int x, int y, int radius, uint32_t color);
void VG_RoundedRect(int x, int y, int width, int height, int radius, uint32_t color);

#endif
