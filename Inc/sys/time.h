/*
    File:       time.h

    Contains:   Management times

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __SYS_TIME__
#define __SYS_TIME__

#include <sys/types.h>

typedef struct Date_t {
    int (*getSeconds)();
    int (*getMinutes)();
    int (*getHours)();
    int (*getDate)();
    int (*getMonth)();
    int (*getYear)();
} Date_t;

typedef struct Time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} __attribute__((packed)) Time_t;

Date_t Date;

extern Time_t GET_TIME();

#endif
