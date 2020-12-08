/*
    File:       color.h

    Contains:   Provide multiple color algorithms

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <vga/color.h>

static const byte_t HexColor[103] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* '0'-'9' */
0, 0, 0, 0, 0, 0, 0,
0, 10, 11, 12, 13, 14, 15, /* 'A'-'F' */
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
10, 11, 12, 13, 14, 15, /* 'a'-'f' */
};

/* Contains: Enter RGBA color in HEX */
uint32_t hex(cstring_t in_code){
    short i, length, number[8];
    char code[8];

    if (*in_code != '#')
        return 0;

    for (length = 1; *(in_code + length) != '\0' && length < 9; code[length - 1] = in_code[length], length++);

    if (length == 4) {
        char code_tmp[2];
        code_tmp[0] = *(code + 1);
        *(code + 2) = *(code + 0);
        code_tmp[1] = *(code + 2);
        *(code + 2) = code_tmp[0];
        *(code + 3) = code_tmp[0];
        *(code + 4) = code_tmp[1];
        *(code + 5) = code_tmp[1];
        length = 6;
        number[6] = 0xf, number[7] = 0xf;
        goto CHANGE;
    }

    if (length != 7 && length != 9)
        return 0;

    number[6] = 0xf, number[7] = 0xf;
    
    length--;

CHANGE:

    for (i = length - 1; i >= 0; number[i] = HexColor[*(code + i)], i--);

    return(((number[0] * 16 + number[1]) << 24) + ((number[2] * 16 + number[3]) << 16) + ((number[4] * 16 + number[5]) << 8) + ((number[6] * 16 + number[7])));
}

/* Contains: Enter RGB color in hsl */
uint32_t hsl(short hue, byte_t saturation, byte_t lightness){
    // uint32_t color_binary;
    // return color_binary;
}

/* Contains: Enter RGBA color in hsl */
uint32_t hsla(short hue, byte_t saturation, byte_t lightness, double a){
    // uint32_t color_binary;
    // return color_binary;
}
