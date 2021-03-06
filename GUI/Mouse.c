/*
    File:       Mouse.c

    Contains:   Provide mouse interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <interrupt.h>
#include <vga/color.h>
#include <vga/mouse.h>
#include <vga/vga.h>

void mouse(void);
void mouse_set(int x, int y, int mode);

extern unsigned int ScreenX, ScreenY;

static char cur_default[32][33] = {
    "--------------------------------",
    "--------------------------------",
    "---**---------------------------",
    "---*+*--------------------------",
    "---*++*-------------------------",
    "---*+++*------------------------",
    "---*++++*-----------------------",
    "---*+++++*----------------------",
    "---*++++++*---------------------",
    "---*+++++++*--------------------",
    "---*++++++++*-------------------",
    "---*+++++*****------------------",
    "---*++*++*----------------------",
    "---*+*-*++*---------------------",
    "---**--*++*---------------------",
    "---*----*++*--------------------",
    "--------*++*--------------------",
    "---------**---------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
    "--------------------------------",
};

/* Contains: Start mouse process */
void mouse(void){
    mouseSet(ScreenX >> 1, ScreenY >> 1, Cursor_Default);
}

/* Contains: Set the mouse's position and mode */
void mouseSet(int x, int y, int mode){
    int i, j;
    for (i = 0; i < 32; i++){
        for (j = 0; j < 32; j++){
            switch (cur_default[i][j]) {
                case '-': break;
                case '+': SetPixel(j + x, i + y, 0, 0, 0, 255);       break;
                case '*': SetPixel(j + x, i + y, 255, 255, 255, 255); break;
                default: break;
            }
        }
    }
}
