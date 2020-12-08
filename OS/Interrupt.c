/*
    File:       Interrupt.c

    Contains:   Interrupts controller

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <assembly.h>
#include <broadcast.h>
#include <interrupt.h>
#include <lib/string.h>

idt_t idt[256];
idtPTR_t idtPTR;
Interrupt_handler_t InterruptHandlers[256];

static void Set_idt(uint8_t Number, uint32_t Base, uint16_t Selector, uint8_t Flags);
extern void IDT_FLUSH(uint32_t);

/* Contains: Initialize interrupt of device */
void Initialize_Interrupt(void){
    Initialize_8259A();
    Initialize_idt();
    Initialize_Pit();

    return;
}

/* Contains: Register interrupt */
void RegisterInterrupt(uint8_t Number, Interrupt_handler_t Handler){
    InterruptHandlers[Number] = Handler;

    return;
}

/* Contains: OS interrupt */
void Call_ISR(SaveRegisters_t *regs){
    if (InterruptHandlers[regs->InterruptNumber]) {
        InterruptHandlers[regs->InterruptNumber](regs);
    } else {
        Debug(DEBUG_NORMAL ,"Unhandled interrupt: %d\n", regs->InterruptNumber);
    }
}

/* Contains: Initialize Interrupt descriptor table */
void Initialize_idt(){
    memset((uint8_t*)&InterruptHandlers, 0, sizeof(Interrupt_handler_t) << 8);

    idtPTR.Limit = sizeof(idt_t) << 8 - 1;
    idtPTR.Base  = (uint32_t)&idt;

    memset((uint8_t *)&idt, 0, sizeof(idt_t) << 8);

    /* 0 ~ 32:  Interrupt Processing for CPU */
    Set_idt( 0, (uint32_t)ISR0,  0x08, 0x8E);
    Set_idt( 1, (uint32_t)ISR1,  0x08, 0x8E);
    Set_idt( 2, (uint32_t)ISR2,  0x08, 0x8E);
    Set_idt( 3, (uint32_t)ISR3,  0x08, 0x8E);
    Set_idt( 4, (uint32_t)ISR4,  0x08, 0x8E);
    Set_idt( 5, (uint32_t)ISR5,  0x08, 0x8E);
    Set_idt( 6, (uint32_t)ISR6,  0x08, 0x8E);
    Set_idt( 7, (uint32_t)ISR7,  0x08, 0x8E);
    Set_idt( 8, (uint32_t)ISR8,  0x08, 0x8E);
    Set_idt( 9, (uint32_t)ISR9,  0x08, 0x8E);
    Set_idt(10, (uint32_t)ISR10, 0x08, 0x8E);
    Set_idt(11, (uint32_t)ISR11, 0x08, 0x8E);
    Set_idt(12, (uint32_t)ISR12, 0x08, 0x8E);
    Set_idt(13, (uint32_t)ISR13, 0x08, 0x8E);
    Set_idt(14, (uint32_t)ISR14, 0x08, 0x8E);
    Set_idt(15, (uint32_t)ISR15, 0x08, 0x8E);
    Set_idt(16, (uint32_t)ISR16, 0x08, 0x8E);
    Set_idt(17, (uint32_t)ISR17, 0x08, 0x8E);
    Set_idt(18, (uint32_t)ISR18, 0x08, 0x8E);
    Set_idt(19, (uint32_t)ISR19, 0x08, 0x8E);
    Set_idt(20, (uint32_t)ISR20, 0x08, 0x8E);
    Set_idt(21, (uint32_t)ISR21, 0x08, 0x8E);
    Set_idt(22, (uint32_t)ISR22, 0x08, 0x8E);
    Set_idt(23, (uint32_t)ISR23, 0x08, 0x8E);
    Set_idt(24, (uint32_t)ISR24, 0x08, 0x8E);
    Set_idt(25, (uint32_t)ISR25, 0x08, 0x8E);
    Set_idt(26, (uint32_t)ISR26, 0x08, 0x8E);
    Set_idt(27, (uint32_t)ISR27, 0x08, 0x8E);
    Set_idt(28, (uint32_t)ISR28, 0x08, 0x8E);
    Set_idt(29, (uint32_t)ISR29, 0x08, 0x8E);
    Set_idt(30, (uint32_t)ISR30, 0x08, 0x8E);
    Set_idt(31, (uint32_t)ISR31, 0x08, 0x8E);

    Set_idt(32, (uint32_t)IRQ0,  0x08, 0x8E);
    Set_idt(33, (uint32_t)IRQ1,  0x08, 0x8E);
    Set_idt(34, (uint32_t)IRQ2,  0x08, 0x8E);
    Set_idt(35, (uint32_t)IRQ3,  0x08, 0x8E);
    Set_idt(36, (uint32_t)IRQ4,  0x08, 0x8E);
    Set_idt(37, (uint32_t)IRQ5,  0x08, 0x8E);
    Set_idt(38, (uint32_t)IRQ6,  0x08, 0x8E);
    Set_idt(39, (uint32_t)IRQ7,  0x08, 0x8E);
    Set_idt(40, (uint32_t)IRQ8,  0x08, 0x8E);
    Set_idt(41, (uint32_t)IRQ9,  0x08, 0x8E);
    Set_idt(42, (uint32_t)IRQ10, 0x08, 0x8E);
    Set_idt(43, (uint32_t)IRQ11, 0x08, 0x8E);
    Set_idt(44, (uint32_t)IRQ12, 0x08, 0x8E);
    Set_idt(45, (uint32_t)IRQ13, 0x08, 0x8E);
    Set_idt(46, (uint32_t)IRQ14, 0x08, 0x8E);
    Set_idt(47, (uint32_t)IRQ15, 0x08, 0x8E);

    /* 33 ~ 255 Make System Call */
    Set_idt(255, (uint32_t)ISR255, 0x08, 0x8E);

    /* Update and Set interrupt descriptor table */
    IDT_FLUSH((uint32_t)&idtPTR);
}

/* Contains: Setting interrupt descriptor table*/
static void Set_idt(uint8_t Number, uint32_t Base, uint16_t Selector, uint8_t Flags) {
    idt[Number].BaseLow  = Base & 0xFFFF;
    idt[Number].BaseHigh = (Base >> 16) & 0xFFFF;

    idt[Number].Selector = Selector;
    idt[Number].SetZero  = 0;

    /* Leave the magic number 0x60 (PORT_KEYDAT), and when the user mode is made later,
       the privilege level of the interrupt gate can be set to 3 with the operation. */
    idt[Number].Flags    = Flags;

    return;
}

/* Contains: IRQ Handler */
void IRQ_Handler(SaveRegisters_t *regs) {
    /* 8259A could only processed Level 8 interruption,
       So Interrupt numbers greater than or equal to 40 are processed from slave slices. */
    if (regs->InterruptNumber >= 40) {
        /* Send reset signal to master chip */
        IO_OUT8(PIC1_ICW1, 0x20);
    }
    /* Send reset signal to master chip */
    IO_OUT8(PIC0_OCW2, 0x20);

    if (InterruptHandlers[regs->InterruptNumber]) {
        InterruptHandlers[regs->InterruptNumber](regs);
    }
}

/* Contains: Initialize 8259A chip */
void Initialize_8259A(void){
    IO_OUT8(PIC0_IMR,  0xff);    /* Prohibit all interruptions */
    IO_OUT8(PIC1_IMR,  0xff);    /* Prohibit all interruptions */

    IO_OUT8(PIC0_ICW1, 0x11);    /* Edge trigger mode */
    IO_OUT8(PIC0_ICW2, 0x20);    /* IRQ0-7 is received by INT 20-27 */
    IO_OUT8(PIC0_ICW3, 0x04);    /* PIC1 is connected by IRQ2 (1 << 2) */
    IO_OUT8(PIC0_ICW4, 0x01);    /* Buffer-free mode */

    IO_OUT8(PIC1_ICW1, 0x11);    /* Edge trigger mode */
    IO_OUT8(PIC1_ICW2, 0x28);    /* IRQ8-15 is received by INT28-2f */
    IO_OUT8(PIC1_ICW3, 0x02);    /* PIC1 is connected by IRQ2 */
    IO_OUT8(PIC1_ICW4, 0x01);    /* Buffer-free mode */

    IO_OUT8(PIC0_IMR,  0xfe);    /* All prohibitions except 11111011 PIC1 */
    IO_OUT8(PIC1_IMR,  0xff);    /* 11111111 prohibits all interruptions */

    return;
}

/* Contains: Initialize Pit chip */
void Initialize_Pit(void){
    /* IO_OUT8 for three, will interrupt the cycle setting for 11932 (0x2e9c),
       the interruption frequency -> 100Hz, that is, 100 interruptions occur in one second. */
    IO_OUT8(PIT_CTRL, 0x34);
    IO_OUT8(PIT_CNT0, 0x9c);
    IO_OUT8(PIT_CNT0, 0x2e);

    return;
}
