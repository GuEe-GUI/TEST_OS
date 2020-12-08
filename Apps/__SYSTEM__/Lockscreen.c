/*
    File:       Lockscreen.c

    Contains:   Lockscreen (App)

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <vga/color.h>
#include <vga/graphics.h>
#include <vga/vga.h>

void InitLockscreen(void);

void Lockscreen(void){
    InitLockscreen();
}

void InitLockscreen(void){
    VG_Rect(0, 0, ScreenX, ScreenY, 0x0058ffff);
    VG_AsciiString((ScreenX - 96) >> 1, ScreenY - 100, "GuEe OS 0.01", 0xffffffff, 0);

    return;
}
