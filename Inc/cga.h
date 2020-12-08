/*
    File:       cga.h

    Contains:   CGA mode interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __CGA__
#define __CGA__

#define CGA_VBE_MEMORY        0x800b8000
#define FOREGROUND_INTENSITY  0x0f

#define CGA_COLOR_NORMAL   0x2f
#define CGA_COLOR_WARNING  0x6f
#define CGA_COLOR_ERROR    0x4f
#define CGA_COLOR_UNKNOW   0x1f

#define SetCurPos(x, y) MOVE_CURSOR((y) * 80 + (x))
#define LineBreak() Print(FOREGROUND_INTENSITY, "\n")
#define ClrScr() CLEAR_SCREEN(1999)

/* OS__Interface.asm */
extern void SET_BACKGROUND_COLOR(unsigned char red, unsigned char green, unsigned char blue);
extern void MOVE_CURSOR(short position);
extern void PUT_CHAR(short position, const char character, unsigned char color);
extern void CLEAR_SCREEN(short eof);
extern void ROLL_SCREEN();

void Print(unsigned char color, const char *string);

#endif
