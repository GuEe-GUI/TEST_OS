/* File: types.h */
#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __cplusplus
#define bool_t _Bool
#define true   1
#define false  0
#else
#define _Bool  bool
#define bool_t bool
#define false  false
#define true   true
#endif

typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef signed int     int32_t;
typedef signed short   int16_t;
typedef signed char    int8_t;
typedef unsigned int   size_t;

#endif
