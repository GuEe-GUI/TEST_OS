/*
    File:       Start.c

    Contains:   Kernel Initialize

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <assembly.h>
#include <broadcast.h>
#include <cga.h>
#include <interrupt.h>
#include <memory.h>
#include <shed.h>
#include <sys/version.h>
#include <timer.h>
#include <vga/graphics.h>
#include <vga/vga.h>

#include "StartImg.inc"

static void Show_VGA_Logo(void);
static void Show_CGA_Logo(void);

/* Contains: Call by _Start and start the Kernel */
int main(void){

    switch (Initialize_Vga()) {
        case 2:
            Show_VGA_Logo();
        break;
        case 1:
            SET_BACKGROUND_COLOR(3, 3, 3);
            Show_CGA_Logo();
        break;
        default:
            POWEROFF();
            HLT();
        break;
    }

    Initialize_Interrupt();
    Initialize_Memory();

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    Initialize_Timer(200);
    asm volatile ("sti");

    return 0;
}

/* Contains: Show the OS start's logo (width = 108px, height = 101px), copyright (width = 165px, height = 20px), 24bit */
static void Show_VGA_Logo(void){
    int x, y, start_x, start_y, end_x, end_y, subs, first_color, second_color, third_color;

    /* Full the screen as background this color (0x14 = 20) */
    VG_Rect(0, 0, ScreenX, ScreenY, 0x141414ff);

    /* Draw logo  (start from x = (ScreenX - 108px) / 2, y = 160px) */
    start_x = (ScreenX - 108) >> 1;
    end_x   = ((ScreenX - 108) >> 1) + 108;

    for (y = 160, subs = 0; y < 261; y++) {
        for (x = start_x; x < end_x; x += 3, subs++) {
            /* Skip the background's color for three piexls */
            if (using_color[logo_info[subs]] == 1315860) continue;
            /* Get the piexls' color */
            first_color  = using_color[logo_info[subs]] >> 16;
            second_color = (using_color[logo_info[subs]] >> 8) & 0xff;
            third_color  = using_color[logo_info[subs]] & 0xff;

            /* Skip the background's color and draw other piexls(first or second or third) */
            if (first_color  != 0x14) SetPixel(    x, y,  first_color,  first_color,  first_color, 0xff);
            if (second_color != 0x14) SetPixel(x + 1, y, second_color, second_color, second_color, 0xff);
            if (third_color  != 0x14) SetPixel(x + 2, y,  third_color,  third_color,  third_color, 0xff);
        }
    }

    /* Draw copyright information (start from x = (ScreenX - 165px) / 2, y = ScreenX - 60px) */
    start_x = (ScreenX - 165) >> 1;
    end_x   = ((ScreenX - 165) >> 1) + 165;
    start_y = ScreenY - 60;
    end_y   = ScreenY - 40;

    for (y = start_y, subs = 0; y < end_y; y++) {
        for (x = start_x; x < end_x; x += 3, subs++) {
            /* Skip the background's color for three piexls */
            if (using_color[copyright_info[subs]] == 1315860) continue;

            /* Get the piexls' color */
            first_color  = using_color[copyright_info[subs]] >> 16;
            second_color = (using_color[copyright_info[subs]] >> 8) & 0xff;
            third_color  = using_color[copyright_info[subs]] & 0xff;

            /* Skip the background's color and draw other  piexls(first or second or third) */
            if (first_color  != 0x14) SetPixel(    x, y,  first_color,  first_color,  first_color, 0xff);
            if (second_color != 0x14) SetPixel(x + 1, y, second_color, second_color, second_color, 0xff);
            if (third_color  != 0x14) SetPixel(x + 2, y,  third_color,  third_color,  third_color, 0xff);
        }
    }

    return;
}

/* Contains: Show the Start message */
static void Show_CGA_Logo(){
    Print(FOREGROUND_INTENSITY, GUEEOS_CGA_LOGO);

    LineBreak();

    Print(FOREGROUND_INTENSITY, VERSION_NUMBER_MESSAGE);
    Print(FOREGROUND_INTENSITY, OS_COPYRIGHT_MESSAGE);

    return;
}
