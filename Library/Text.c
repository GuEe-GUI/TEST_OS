/*
    File:       Text.c

    Contains:   Text function

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/text.h>

string_t ToString(cstring_t string, ...){
    int StringLength = 0;
    static char Result[8192] = {0};
    string_t string_tmp = (string_t)NULL;
    va_list Arg_ptr;

    va_start(Arg_ptr, string);

    while (*string != '\0') {
        if (*string != '%') {
            Result[StringLength++] = *(string++);
            continue;
        }

        string++;

        switch (*string) {
            case 'd': case 'D': {
                string_tmp = ToDec_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'x': case 'X': {
                string_tmp = ToHex_String(va_arg(Arg_ptr, int), false);
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'p': {
                string_tmp = ToHex_String(va_arg(Arg_ptr, int), true);
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'b': case 'B': {
                string_tmp = ToBin_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'o': case 'O': {
                string_tmp = ToOct_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'c': case 'C': {
                Result[StringLength++] = (char)va_arg(Arg_ptr, int);
                break;
            }
            case 's': case 'S': {
                string_tmp = va_arg(Arg_ptr, string_t);
                while (*string_tmp != '\0') {
                    Result[StringLength++] = *string_tmp++;
                }
                break;
            }
            case '%': {
                Result[StringLength++] = '%';
                break;
            }
            /* It isn't enough */
            default: Result[StringLength++] = *string; break;
        }

        string++;
    }

    va_end(Arg_ptr);

    Result[StringLength] = '\0';

    return Result;
}

string_t VToString(cstring_t string, va_list Arg_ptr){
    int StringLength = 0;
    static char Result[8192] = {0};
    string_t string_tmp = (string_t)NULL;

    while (*string != '\0') {
        if (*string != '%') {
            Result[StringLength++] = *(string++);
            continue;
        }

        string++;

        switch (*string) {
            case 'd': case 'D': {
                string_tmp = ToDec_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'x': case 'X': {
                string_tmp = ToHex_String(va_arg(Arg_ptr, int), false);
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'p': {
                string_tmp = ToHex_String(va_arg(Arg_ptr, int), true);
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'b': case 'B': {
                string_tmp = ToBin_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'o': case 'O': {
                string_tmp = ToOct_String(va_arg(Arg_ptr, int));
                while (*string_tmp != '\0') Result[StringLength++] = *(string_tmp++);
                break;
            }
            case 'c': case 'C': {
                Result[StringLength++] = (char)va_arg(Arg_ptr, int);
                break;
            }
            case 's': case 'S': {
                string_tmp = va_arg(Arg_ptr, string_t);
                while (*string_tmp != '\0') {
                    Result[StringLength++] = *string_tmp++;
                }
                break;
            }
            case '%': {
                Result[StringLength++] = '%';
                break;
            }
            /* It isn't enough */
            default: Result[StringLength++] = *string; break;
        }

        string++;
    }

    va_end(Arg_ptr);

    Result[StringLength] = '\0';

    return Result;
}

string_t ToDec_String(int DecimalNumber){
    char Result_tmp[32];
    static char Result[34];
    const char DecChar[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int i = 0, j = 0;

    if (DecimalNumber < 0) Result[i++] = '-';

    while (DecimalNumber > 0) {
        *(Result_tmp + j) = DecChar[DecimalNumber % 10];
        j++;
        DecimalNumber /= 10;
    }

    for (j--; j >= 0; Result[i++] = Result_tmp[j--]);

    Result[i] = '\0';

    return Result;
}

string_t ToOct_String(int DecimalNumber){
    if (DecimalNumber < 0) return "Error: This number is smaller than 0";

    char Result_tmp[32];
    static char Result[34];
    const char OctChar[9] = {'0', '1', '2', '3', '4', '5', '6', '7', '8'};
    int i = 0, j = 0;

    Result[i++] = '0';

    while (DecimalNumber > 0) {
        *(Result_tmp + j) = OctChar[DecimalNumber % 8];
        j++;
        DecimalNumber >>= 3;
    }

    for (j--; j >= 0; Result[i++] = Result_tmp[j--]);

    return Result;
}

string_t ToHex_String(int DecimalNumber, bool_t sign){
    if (DecimalNumber < 0) return "Error: This number is small than 0";
    char Result_tmp[32];
    static char Result[35];
    const char HexChar[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i = 0, j = 0;

    if (sign == true) {
        Result[i++] = '0';
        Result[i++] = 'x';
    }

    while (DecimalNumber > 0) {
        *(Result_tmp + j) = HexChar[DecimalNumber % 16];
        j++;
        DecimalNumber >>= 4;
    }

    for (j--; j >= 0; Result[i++] = Result_tmp[j--]);

    Result[i] = '\0';

    return Result;
}

string_t ToBin_String(int DecimalNumber){
    if (DecimalNumber < 0) return "The parameter is error!";

    char Result_tmp[32];
    static char Result[34];
    int i = 0, j = 0;

    while (DecimalNumber > 0) {
        *(Result_tmp + j) = (DecimalNumber % 2) ? '1' : '0';
        j++;
        DecimalNumber >>= 1;
    }

    for (j--; j >= 0; Result[i++] = Result_tmp[j--]);

    Result[i++] = 'b';
    Result[i]   = '\0';

    return Result;
}
