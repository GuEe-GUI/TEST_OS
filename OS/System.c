/*
    File:       System.c

    Contains:   System running

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <assembly.h>
#include <cga.h>
#include <interrupt.h>
#include <vga/vga.h>

extern void Lockscreen(void);
extern void Captain(void);
extern void Console(void);
extern void ShowSystemTime(void);

/* Contains: This operating system's loop */
void System(){

    if (VGA_Toggle){
        Lockscreen();
        Captain();
    } else {
        Print(CGA_COLOR_NORMAL, "\n[Console Process]\n");
        Console();
    }

    while (true) {
        //ShowSystemTime();
    }

    return;
}
