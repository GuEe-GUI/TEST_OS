/* File: text.h */
#ifndef _TEXT_H_
#define _TEXT_H_

#include <types.h>

char* to_Dec_string(int decimal_number);
char* to_Oct_string(int decimal_number);
char* to_Hex_string(int decimal_number, bool_t sign);
char* to_Bin_string(int decimal_number);

#endif
