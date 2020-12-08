/*
    File:       encoding.h

    Contains:   Provide encoding and decoding methods

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __LIB_ENCODING__
#define __LIB_ENCODING__

#include <sys/types.h>

string_t Encode_Base64(char in_string[]);
string_t Decode_Base64(char in_string[]);

#endif
