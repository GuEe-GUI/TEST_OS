/*
    File:       Console.c

    Contains:   Console Process

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <cga.h>
#include <sys/types.h>

byte_t consoleColor       = FOREGROUND_INTENSITY;
byte_t backgroundColor[3] = {0};

void Type(void);

/* Contains: Default console process */
void Console() {

    Type();

    return;
}

/* Contains: Default console process of scan io */
void Type(void){
    Print(consoleColor, "Type:\\>");
}
