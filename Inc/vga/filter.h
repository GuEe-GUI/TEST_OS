/*
    File:       filter.h

    Contains:   Coloring function

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_FILTER__
#define __VGA_FILTER__

#include <vga/vga.h>

/* VF --> VGA Filter */

void VF_Blur(int x, int y, int width, int height, int radius);
void VF_Brightness(int x, int y, int width, int height, uint8_t value);
void VF_Contrast(int x, int y, int width, int height, uint8_t value);
void VF_DropShadow(int x, int y, int width, int height);
void VF_Grayscale(int x, int y, int width, int height, uint8_t value);
void VF_HueRotate(int x, int y, int width, int height, uint16_t deg);
void VF_Invert(int x, int y, int width, int height, uint8_t value);
void VF_Opacity(int x, int y, int width, int height, uint8_t value);
void VF_Saturate(int x, int y, int width, int height, uint8_t value);
void VF_Sepia(int x, int y, int width, int height, uint8_t value);

#endif
