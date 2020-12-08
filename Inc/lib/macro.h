/*
    File:       macro.h

    Contains:   Provide common macro definition

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __LIB_MACRO__
#define __LIB_MACRO__

/* Function: Take the y(th) bit of the bit of x */
#define Bitof(x, y) ((x) >> ((y) - 1) & 1)

/* Function: Exchange the values of two variables */
#define Swap(type, x, y) do {\
    type tmp = x;            \
    x = y;                   \
    y = tmp;                 \
} while(0)

/* Function: The char change */
#define Tolower(char)   ((char)-'A'+'a')
#define Toupper(char)   ((char)-'a'+'A')

#endif
