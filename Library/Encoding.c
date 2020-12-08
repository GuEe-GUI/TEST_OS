/*
    File:       encoding.c

    Contains:   Provide encoding and decoding methods

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/encoding.h>

static const unsigned char outbase64[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

static const unsigned char inbase64[123] = {
    [43] = 62, /* '+' */
    [47] = 63, /* '/' */
    [48] = 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, /* '0'-'9' */
    [65] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, /* 'A'-'Z' */
    [97] = 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, /* 'a'-'z' */
};

string_t Encode_Base64(char in_string[]){
    static char out_string[1048576];
    int pos = 0, sub = 0;

    for (; in_string[pos] != '\0'; pos++){
        if (pos >= 786432) return "out_base64(String): String too long, don't set more than 786432 chars!";
    }

    pos = 0;

    while (in_string[sub] != '\0'){
        if (in_string[sub] == '\0' || in_string[sub + 1] == '\0' || in_string[sub + 2] == '\0'){
            if (in_string[sub] == '\0'){
                out_string[pos] = '\0';
                return out_string;
            } else {
                out_string[pos]     = outbase64[(byte_t)in_string[sub] >> 2];
                out_string[pos + 1] = outbase64[((byte_t)(in_string[sub] << 6) >> 2) + (byte_t)(in_string[sub + 1] >> 4)];
            }

            if (in_string[sub + 1] == '\0'){
                out_string[pos + 2] = '=';
                out_string[pos + 3] = '=';
                out_string[pos + 4] = '\0';
                return out_string;
            } else {
                out_string[pos + 2] = outbase64[((byte_t)(in_string[sub + 1] << 4) >> 2) + (byte_t)(in_string[sub + 2] >> 6)];
            }

            if (in_string[sub + 2] == '\0'){
                out_string[pos + 3] = '=';
                out_string[pos + 4] = '\0';
                return out_string;
            }
        }

        out_string[pos++] = outbase64[(byte_t)in_string[sub] >> 2];
        out_string[pos++] = outbase64[((byte_t)(in_string[sub] << 6) >> 2) + (byte_t)(in_string[sub + 1] >> 4)];
        out_string[pos++] = outbase64[((byte_t)(in_string[sub + 1] << 4) >> 2) + (byte_t)(in_string[sub + 2] >> 6)];
        out_string[pos++] = outbase64[((byte_t)(in_string[sub + 2] << 2) >> 2)];

        sub += 3;
    }

    out_string[pos] = '\0';

    return (string_t)out_string;

}

string_t Decode_Base64(char in_string[]){
    static char out_string[786432], in_string_tmp[1048576];
    int pos = 0, sub = 0;

    for (;in_string[pos] != '\0' || pos >= 1048576; in_string_tmp[pos] = inbase64[in_string[pos]], pos++);

    if (pos >= 1048576)
        return "in_base64(String): String too long, don't set more than 1048576 chars!";
    else if (pos % 4 != 0)
        return "in_base64(Error): The number of chars should be a multiple of 4!";
    else
        pos = 0;

    while (in_string[sub] != '\0'){
        out_string[pos++] = (byte_t)((in_string_tmp[sub] << 2) + (in_string_tmp[sub + 1] >> 4));
        out_string[pos++] = (byte_t)((in_string_tmp[sub + 1] << 4) + (in_string_tmp[sub + 2] >> 2));
        out_string[pos++] = (byte_t)((in_string_tmp[sub + 2] << 6) + in_string_tmp[sub + 3]);

        sub += 4;
    }

    out_string[pos] = '\0';

    return out_string;
}
