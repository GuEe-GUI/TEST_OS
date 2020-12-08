/*
    File:       ARDS.h

    Contains:   ARDS read and initialization

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __ARDS__
#define __ARDS__

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#define ARDS_N_ADDRESS  0x80005010  /* The storage address of ARDS's number */
#define ARDS_ADDRESS    0x80005050  /* The storage address of ARDS          */

typedef struct ARDS_t {
    uint32_t BaseAddrLow;       /* Base address low 32bits    */
    uint32_t BaseAddrHigh;      /* Base address high 32bits   */
    uint32_t LengthLow;         /* Length low 32bits (bytes)  */
    uint32_t LengthHigh;        /* Length high 32bits (bytes) */
    uint32_t Type;              /* Type of this memory (could be used if Type equal to 1) */
} ARDS_t;

void Initialize_ARDS();

#endif
