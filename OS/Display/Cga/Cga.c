/*
    File:       Cga.c

    Contains:   CGA mode interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <cga.h>

static unsigned short stringTotalLength = 0;

/* Contains: output the string in CGA mode */
void Print(unsigned char color, const char *string){

    while (*string != '\0') {
        /* Ascii char: ' ' */
        if (*string < 32) {
            switch (*string) {
                case '\n': {
                    if (stringTotalLength >= 1920) {
                        stringTotalLength = 1920;
                        ROLL_SCREEN();
                        goto END;
                    }
                    stringTotalLength = (stringTotalLength / 80 + 1) * 80;
                    MOVE_CURSOR(stringTotalLength);
                    goto END;
                }
                case '\t': {
                    stringTotalLength += 4 - (stringTotalLength % 4);
                    MOVE_CURSOR(stringTotalLength);
                    goto END;
                }
                case '\b': {
                    if (stringTotalLength % 80 != 0) {
                        MOVE_CURSOR(--stringTotalLength-1);
                    }
                    goto END;
                }
                case '\r': {
                    stringTotalLength = (stringTotalLength / 80) * 80;
                    MOVE_CURSOR(stringTotalLength);
                    goto END;
                }
                default: break;
            }
        }

        PUT_CHAR(++stringTotalLength, *string, color);
        if (stringTotalLength >= 2000) {
            ROLL_SCREEN();
            stringTotalLength -= 80;
        }

    END:
        string++;
    }

    return;
}
