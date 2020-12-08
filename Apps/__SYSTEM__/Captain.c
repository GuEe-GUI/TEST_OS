/*
    File:       Captain.c

    Contains:   A Explorer: Captain (App)

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/encoding.h>
#include <sys/time.h>
#include <vga/color.h>
#include <vga/mouse.h>
#include <vga/graphics.h>
#include <vga/vga.h>

bool_t InitLauncherED = false;
static Time_t systemTime;

bool_t InitLauncher(void);
void DrawLauncher(void);
void ShowSystemTime(void);

void Captain(void){
    InitLauncherED = InitLauncher();

    mouse();
}

bool_t InitLauncher(){

    if (InitLauncherED) return true;

    DrawLauncher();
    mouse();

    return true;
}

void DrawLauncher(void){
    int set_x, set_y, set_color;

    /* Desktop background */
    VG_Rect(0, 0, ScreenX, ScreenY, rgb(255, 172, 56));

    /* Frame of tesktopbar */
    VG_Rect(0, 0, ScreenX, 28, 0xffffffd8);

    /* Shadow of tesktopbar */
    for (set_x = ScreenX; set_x >= 0; set_x--) {
        SetPixel(set_x, 28, 0, 0, 0, 90);
        SetPixel(set_x, 29, 0, 0, 0, 70);
        SetPixel(set_x, 30, 0, 0, 0, 50);
        SetPixel(set_x, 31, 0, 0, 0, 30);
        SetPixel(set_x, 32, 0, 0, 0, 20);
        SetPixel(set_x, 33, 0, 0, 0, 10);
    }

    VG_AsciiString(101, 150, "This is a window, it will become better!", hex("#3a3a3a"), 0);
    VG_AsciiString(101, 166, (cstring_t)Encode_Base64("Man"), hex("#3a3a3a"), 0);
    VG_AsciiString(101, 182, (cstring_t)Decode_Base64(Encode_Base64("Man")), hex("#3a3a3a"), 0);
    VG_AsciiString(195, 450, "Written by: GUI -- 2018/03/07", hex("#3a3a3a"), 0);

    VG_AsciiString(0, 0, "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", hex("#000"), 0);

    VG_Rect(0, 0, 300, 300, rgba(20,20,20,0.5));
    VG_Rect(300, 300, 300, 300, hex("#1414147d"));
    VG_Circle(150, 150, 150, hex("#ff14147d"));
    VG_Circle(400, 200, 100, hex("#0000ffff"));
    VG_Ring(450, 300, 100, hex("#00ff00ff"));
    return;
}

void ShowSystemTime(void) {
    GET_TIME();
    //systemTime = GET_TIME();
    return;
}
