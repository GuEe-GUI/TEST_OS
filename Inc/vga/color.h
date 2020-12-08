/*
    File:       color.h

    Contains:   Provide multiple color algorithms and code

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_COLOR__
#define __VGA_COLOR__

#include <lib/math.h>
#include <sys/types.h>

/* Warning: must be used constant when call the macros */
#define Red(x)	   (((x) >> 24) & 0xff)     /* (((x) & 0xff000000) >> 24) */
#define Green(x)   (((x) >> 16) & 0xff)     /* (((x) & 0x00ff0000) >> 16) */
#define Blue(x)    (((x) >>  8) & 0xff)     /* (((x) & 0x0000ff00) >> 8)  */
#define Alpha(x)   (((x) & 0xff))           /* (((x) & 0x000000ff))       */

#define rgb(r, g, b)      (long int)(((r) << 24) +((g) << 16) + ((b) << 8) + 0xff)
#define rgba(r, g, b, a)  (long int)(((r) << 24) + ((g) << 16) + ((b) << 8) + (Mid(0, (a), 1) * 0xff))
#define grayed(color)     (long int)(((color) << 24) + ((color) << 16) + ((color) << 8) + 0xff)
#define grayedA(color, a) (long int)(((color) << 24) + ((color) << 16) + ((color) << 8) + ((a) * 0xff))

uint32_t hex(cstring_t code);
uint32_t hsl(short hue, byte_t saturation, byte_t lightness);
uint32_t hsla(short hue, byte_t saturation, byte_t lightness, double a);

#define COLOR_TRANSPARENT 0
#define COLOR_RED         0xff0000ff
#define COLOR_GREEN       0x00ff00ff
#define COLOR_BLUE        0x0000ffff
#define COLOR_WHITE       0xffffffff
#define COLOR_BLACK       0x000000ff

#define COLOR_LIGHTPINK   0xffb6c1ff
#define COLOR_PINK        0xffc0cbff
#define COLOR_CRIMSON     0xdc143cff
#define COLOR_PURPLE      0x800080ff
#define COLOR_FUCHSIA     0xff00ffff
#define COLOR_DODERBLUE   0x1e90ffff
#define COLOR_AZURE       0xf0ffffff
#define COLOR_CYAN        0x00ffffff
#define COLOR_YELLOW      0xffff00ff
#define COLOR_GOLD        0xffd700ff
#define COLOR_ORANGE      0xffa500ff
#define COLOR_CHOCOLATE   0xd2691eff
#define COLOR_CORAL       0xff7f50ff
#define COLOR_ORANGERED   0xff4500ff
#define COLOR_TOMATO      0xff6347ff
#define COLOR_SNOW        0xfffafaff
#define COLOR_BROWN       0xa52a2aff
#define COLOR_WHITESMOKE  0xf5f5f5ff
#define COLOR_LIGHTGREY   0xd3d3d3ff
#define COLOR_SILVER      0xc0c0c0ff
#define COLOR_DARKGRAY    0xa9a9a9ff
#define COLOR_DIMGRAY     0x696969ff
#define COLOR_GRAY        0x808080ff

#endif
