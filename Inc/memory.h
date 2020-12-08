/*
    File:       memory.h

    Contains:   Memory management

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __MEMORY__
#define __MEMORY__

#include <sys/types.h>

typedef struct MemoryObject_t {
    uint32_t TotalMemorySize;       /* Unit of memory-sized: MB */
    uint32_t SystemUsing;
    uint32_t Reserved;
    uint32_t RAM;
    uint32_t RAMUsing;
} MemoryObject_t;

extern int TotalMemoryBytes;

void Initialize_Memory(void);

#endif
