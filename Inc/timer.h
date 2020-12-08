/*
    File:       timer.h

    Contains:   Clock interruption

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __TIMER__
#define __TIMER__

#include <interrupt.h>
#include <sys/types.h>

void TimerCallback(SaveRegisters_t *regs);
void Initialize_Timer(uint32_t frequency);

#endif
