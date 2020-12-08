/*
    File:       Window.c

    Contains:   Drawing windows form handles and interfaces in vga mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/macro.h>
#include <vga/font.h>
#include <vga/color.h>
#include <vga/graphics.h>
#include <vga/vga.h>
#include <vga/window.h>

#include "Resources/FormStyle.h"

/*
* | Frame | TitleBackground | WidgetButton(Font) | WidgetButtonBackground | Main |
*/
static byte_t WindowLight[5] = {227, 242,  58, 217, 250},
              WindowDark[5]  = { 69,  36, 242,  61,  46};

void Window(void){
    return;
}

/* Contains: Create a window */
void CreatWindow(Form_t form, int x, int y, int width, int height, bool_t WindowLightStyle){
    byte_t *StyleColor = WindowLight;
    if (!WindowLightStyle) {
        StyleColor = WindowDark;
    }

    /* window's Frame */
    VG_Frame(x++, y++, width, height - 1, rgb(StyleColor[0], StyleColor[0], StyleColor[0]));
    /* windows's TitleBackground */
    VG_Rect(x, y, width - 2, 28, rgb(StyleColor[1], StyleColor[1], StyleColor[1]));
    /* windows's Icon */
    DrawWindowWidget(x + 7, y + 7, Icon, hex("#0099ef"));
    /* windows's Title */
    VG_AsciiString(x + 28, y + 5, form.WindowTitle, rgb(StyleColor[2], StyleColor[2], StyleColor[2]), COLOR_TRANSPARENT);
    /* window's Widget Min Button */
    DrawWindowWidget(x + width - 69, y + 7,      Min, rgb(StyleColor[2], StyleColor[2], StyleColor[2]));
    /* window's Widget Max Button */
    DrawWindowWidget(x + width - 46, y + 7,      Max, rgb(StyleColor[2], StyleColor[2], StyleColor[2]));
    DrawWindowWidget(x + width - 46, y + 7,  MaxBack, rgb(StyleColor[3], StyleColor[3], StyleColor[3]));
    /* window's Widget Exit Button */
    DrawWindowWidget(x + width - 23, y + 7,     Exit, rgb(StyleColor[2], StyleColor[2], StyleColor[2]));
    DrawWindowWidget(x + width - 23, y + 7, ExitBack, rgb(StyleColor[3], StyleColor[3], StyleColor[3]));
    /* window's Main */
    VG_Rect(x, y + 28, width - 2, height - 30, rgb(StyleColor[4], StyleColor[4], StyleColor[4]));

    return;
}

/* Contains: Create window's control button */
void DrawWindowWidget(int x, int y, int mode[], long int color){
    int set_x, set_y, subs_mode, subs_bit, y_tmp = y + 14, x_tmp = x + 14;

    for (set_y = y, subs_mode = 0; set_y < y_tmp; set_y++, subs_mode++){
        for (set_x = x, subs_bit = 0; set_x < x_tmp; set_x++, subs_bit++){
            /* Skip the background's color */
            if (Bitof(mode[subs_mode], 14 - subs_bit) == 0) continue;
                /* Draw a piexl */
                SetPixel(set_x, set_y, Red(color), Green(color), Blue(color), Alpha(color));
        }
    }

    return;
}
