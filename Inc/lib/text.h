/*
    File:       text.h

    Contains:   Text function

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __LIB_TEXT__
#define __LIB_TEXT__

#include <lib/arg.h>
#include <sys/types.h>
//itoa(a, s, 2);
string_t ToString(cstring_t string, ...);
string_t VToString(cstring_t string, va_list Arg_ptr);
void     ParseInt(var_t object);
string_t ToDec_String(int DecimalNumber);
string_t ToOct_String(int DecimalNumber);
string_t ToHex_String(int DecimalNumber, bool_t sign);
string_t ToBin_String(int DecimalNumber);

#endif
