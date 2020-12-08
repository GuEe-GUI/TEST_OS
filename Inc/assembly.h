/*
    File:       assembly.h

    Contains:   Operating Assembly IO (Input/Output)

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __ASSEMBLY__
#define __ASSEMBLY__

#define CLI() __asm__ ("cli\n\t"::)
#define HLT() __asm__ ("hlt\n\t"::)
#define STI() __asm__ ("sti\n\t"::)
#define NOP() __asm__ ("nop\n\t")

/* OS__IO.asm */
extern int  IO_IN8(int port);
extern void IO_OUT8(int port, int data);
extern int  IO_LOAD_EFLAGS(void);
extern void IO_STORE_EFLAGS(int eflags);

/* OS__Power__PowerMgr.asm */
extern void POWEROFF();

#endif
