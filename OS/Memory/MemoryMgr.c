/*
    File:       MemoryMgr.c

    Contains:   Memory Manager

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <memory.h>
#include <broadcast.h>

#include "ARDS.h"

static MemoryObject_t MemoryObject;
static ARDS_t        *ARDS;

int totalMemoryBytes = 0;

/* Contains: Initialize the memory information */
void Initialize_Memory(void){

    Initialize_ARDS();

    MemoryObject.TotalMemorySize = totalMemoryBytes >> 20; /* 1024 * 1024 = 2^(10+10) */
    MemoryObject.SystemUsing     = 0;
    MemoryObject.Reserved        = 0;
    MemoryObject.RAM             = 0;
    MemoryObject.RAMUsing        = 0;

    // Debug(DEBUG_ERROR, "Memory size: %dMB", MemoryObject.TotalMemorySize);

    return;
}

/* Contains: Get the max memory provided by the device */
void Initialize_ARDS(){
    uint16_t ARDS_Number = *(int16_t*)ARDS_N_ADDRESS;
    int number = ARDS_Number, sum;

    ARDS = (ARDS_t*)ARDS_ADDRESS;

    for (; number >= 0; number--) {
        /* Type of this memory (could be used if Type equal to 1) */
        if (ARDS->Type == 1) {
            sum = ARDS->BaseAddrLow + ARDS->LengthLow;
            /* Get the max sizes */
            if (sum > totalMemoryBytes) {
                totalMemoryBytes = sum;
            }
        }
    }

    // Debug(DEBUG_ERROR, "ARDS number: %d", ARDS_Number);

    return;
}
