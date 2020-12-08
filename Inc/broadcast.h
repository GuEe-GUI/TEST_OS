/*
    File:       broadcast.h

    Contains:   Notification Transfer in Operating System

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __BROADCAST__
#define __BROADCAST__

#include <lib/arg.h>
#include <sys/types.h>

#define DEBUG_NORMAL    0
#define DEBUG_WARNING   1
#define DEBUG_ERROR     2
#define DEBUG_UNKNOW    3

void Debug(byte_t type, cstring_t string, ...);

#endif
