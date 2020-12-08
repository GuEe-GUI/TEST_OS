/*
    File:       Broadcast.c

    Contains:   Notification Transfer in Operating System

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <broadcast.h>
#include <cga.h>
#include <lib/text.h>
#include <vga/color.h>
#include <vga/graphics.h>
#include <vga/vga.h>

static const lint_t VGA_DebugColor[4] = {
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_BLUE
};

static const byte_t CGA_DebugColor[4] = {
    CGA_COLOR_NORMAL,
    CGA_COLOR_WARNING,
    CGA_COLOR_ERROR,
    CGA_COLOR_UNKNOW
};

static short VGA_DebugViewRoll = 0;

/* Contains: Debug arguments for the OS in every mode */
void Debug(byte_t type, cstring_t string, ...){
    string_t getString = (string_t)NULL;
    va_list arg_ptr;

    va_start(arg_ptr, string);

    getString = VToString(string, arg_ptr);

    va_end(arg_ptr);

    if (VGA_Toggle) {
        /* Need to line wrap in VGA mode, so couldn't support '\n' */
        VG_AsciiString(0, VGA_DebugViewRoll, (cstring_t)getString, VGA_DebugColor[type], COLOR_BLACK);
    } else {
        /* No line wrap in CGA mode*/
        Print(CGA_DebugColor[type], (cstring_t)getString);
    }

    if (VGA_Toggle) {
        VGA_DebugViewRoll += 16;
        if (VGA_DebugViewRoll >= ScreenY >> 1) {
            VGA_DebugViewRoll = 0;
        }
    }

    return;
}
