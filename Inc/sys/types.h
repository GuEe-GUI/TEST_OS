/*
    File:       types.h

    Contains:   Provide variable type interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __SYS_TYPES__
#define __SYS_TYPES__

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef ERROR
#define ERROR (-2)
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

typedef unsigned long long int uint64_t;
typedef unsigned int           uint32_t;
typedef unsigned short         uint16_t;
typedef unsigned char          uint8_t;
typedef signed long long int   int64_t;
typedef signed int             int32_t;
typedef signed short           int16_t;
typedef signed char            int8_t;
typedef unsigned int           size_t;

typedef const char*            cstring_t;
typedef char*                  string_t;
typedef unsigned long          ulong_t;
typedef long int               lint_t;
typedef unsigned char          byte_t;
typedef void*                  var_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Need change when compile in c++ */

#ifdef __cplusplus
}
#endif

#endif
