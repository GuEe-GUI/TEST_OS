/*
    File:       Start.c

    Contains:   Clock interruption

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <assembly.h>
#include <broadcast.h>
#include <timer.h>

/* Contains: Debug the timer */
void TimerCallback(SaveRegisters_t *regs) {
    static uint32_t tick = 0;
    Debug(DEBUG_NORMAL, "Tick: %d\n", tick++);
}

/* Contains: Initialize the timer */
void Initialize_Timer(uint32_t frequency) {
    RegisterInterrupt(IRQ_0, TimerCallback);

    /* Intel 8253/8254 PIT chip I/O port address range is 40h~43h,
       Frequency is the number of interrupts per second. */
    uint32_t divisor = 1193180 / frequency;

    /*
       D7 D6 D5 D4 D3 D2 D1 D0
       0  0  1  1  0  1  1  0
       That's 36 H.
       Setting up 8253/8254 chip works under mode 3
    */
    IO_OUT8(0x43, 0x36);

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t hign = (uint8_t)((divisor >> 8) & 0xFF);

    IO_OUT8(0x40, low);
    IO_OUT8(0x40, hign);
}
