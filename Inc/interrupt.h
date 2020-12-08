/*
    File:       interrupt.h

    Contains:   Interrupt controller

    Written by: GUI

    Copyright:	(C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __INTERRUPT__
#define __INTERRUPT__

#include <sys/types.h>

void Initialize_Interrupt(void);

// Interrupt descriptor table
typedef struct idt_t {
    uint16_t BaseLow;                /* Interrupt Handler Address 15-0 Bits */
    uint16_t Selector;               /* Object code segment descriptor selector */
    uint8_t  SetZero;                /* Paragraph 0 */
    uint8_t  Flags;                  /* Some flags */
    uint16_t BaseHigh;               /* Interrupt Processing Function Addresses 31-16 Bits */
} __attribute__((packed)) idt_t;

// IDTR
typedef struct idtPTR_t {
    uint16_t Limit;                  /* Limit length */
    uint32_t Base;                   /* Base address */
} __attribute__((packed)) idtPTR_t;

/* Contains: Initialize Interrupt descriptor table*/
void Initialize_idt();

typedef struct SaveRegisters_t {
    uint32_t ds;                    /* Data segment descriptors for saving users */
    uint32_t edi;                   /* Push in by pusha command (edi ~ eax) */
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t InterruptNumber;
    uint32_t ErrorCode;
    uint32_t eip;                   /* Push Automatic by CPU (etc.) */
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t ss;
} __attribute__((packed)) SaveRegisters_t;

/* Define interrupt handler pointer */
typedef void (*Interrupt_handler_t)(SaveRegisters_t*);

/* Create an interrupt handler */
void RegisterInterrupt(uint8_t Number, Interrupt_handler_t Handler);

/* Call the interrupt handler (Interrupt Service Routine)*/
extern void Call_ISR(SaveRegisters_t *regs);

/* Interrupt Handler: NO.0 ~ 19 are Abnormal interruption of CPU */
extern void ISR0(void);     /*  0 #DE Divide by 0 anomaly                          */
extern void ISR1(void);     /*  1 #DB Debug anomaly                                */
extern void ISR2(void);     /*  2 NMI                                              */
extern void ISR3(void);     /*  3 BP  Breakpoint Exception                         */
extern void ISR4(void);     /*  4 #OF Overflow                                     */
extern void ISR5(void);     /*  5 #BR Cross-border occurs when referring to arrays */
extern void ISR6(void);     /*  6 #UD Invalid or undefined opcodes                 */
extern void ISR7(void);     /*  7 #NM Equipment unavailable                        */
extern void ISR8(void);     /*  8 #DF Dual failures (Error codes)                  */
extern void ISR9(void);     /*  9 ___ Coprocessor Cross-Section Operations         */
extern void ISR10(void);    /* 10 #TS Invalid TSS (Error codes)                    */
extern void ISR11(void);    /* 11 #NP Segment not present (Error codes)            */
extern void ISR12(void);    /* 12 #SS Stack error (Error codes)                    */
extern void ISR13(void);    /* 13 #GP Conventional protection (Error codes)        */
extern void ISR14(void);    /* 14 #PF Page failure (Error codes)                   */
extern void ISR15(void);    /* 15 ___ Was Occupied by CPU                          */
extern void ISR16(void);    /* 16 #MF Floating Point Processing Unit Error         */
extern void ISR17(void);    /* 17 #AC Alignment check                              */
extern void ISR18(void);    /* 18 #MC Machine Inspection                           */
extern void ISR19(void);    /* 19 #XM SIMD (Single Instruction Multiple Data) Floating point anomaly */
/* Interrupt Handler: NO.20 ~ 31 are Occupied by Intel */
extern void ISR20(void);
extern void ISR21(void);
extern void ISR22(void);
extern void ISR23(void);
extern void ISR24(void);
extern void ISR25(void);
extern void ISR26(void);
extern void ISR27(void);
extern void ISR28(void);
extern void ISR29(void);
extern void ISR30(void);
extern void ISR31(void);
/* Interrupt Handler: NO.32 ~ 255 are User-defined exceptions */
extern void ISR255(void);

/* Loading IDTR */
extern void IDT_FLUSH(uint32_t);

/* 8259A-All */
#define PIC0_ICW1   0x20

/* 8259A-Master */
#define PIC0_OCW2   0x20
#define PIC0_IMR    0x21
#define PIC0_ICW2   0x21
#define PIC0_ICW3   0x21
#define PIC0_ICW4   0x21
#define PIC1_ICW1   0xa0

/* 8259A-Slave */
#define PIC1_OCW2   0xa0
#define PIC1_IMR    0xa1
#define PIC1_ICW2   0xa1
#define PIC1_ICW3   0xa1
#define PIC1_ICW4   0xa1

/* Contains: Initialize 8259A chip */
void Initialize_8259A(void);

/* IRQ Handler */
void IRQ_Handler(SaveRegisters_t *regs);

/* IRQ (Interrupt Request) */
#define  IRQ_0   32
#define  IRQ_1   33
#define  IRQ_2   34
#define  IRQ_3   35
#define  IRQ_4   36
#define  IRQ_5   37
#define  IRQ_6   38
#define  IRQ_7   39
#define  IRQ_8   40
#define  IRQ_9   41
#define IRQ_10   42
#define IRQ_11   43
#define IRQ_12   44
#define IRQ_13   45
#define IRQ_14   46
#define IRQ_15   47

/* Contains: Interrupt Request */
extern void  IRQ0(void);    /* Computer System Timer                               */
extern void  IRQ1(void);    /* keyboard                                            */
extern void  IRQ2(void);    /* Connecting with IRQ9, MPU-401 MD is used            */
extern void  IRQ3(void);    /* serial device                                       */
extern void  IRQ4(void);    /* serial device                                       */
extern void  IRQ5(void);    /* Suggested Sound Card Use                            */
extern void  IRQ6(void);    /* Floppy Drive Transmission Control                   */
extern void  IRQ7(void);    /* Use of Printer Transfer Control                     */
extern void  IRQ8(void);    /* Real-time clock                                     */
extern void  IRQ9(void);    /* Connect with IRQ2 and can be set to other hardware  */
extern void IRQ10(void);    /* Suggested Network Card Use                          */
extern void IRQ11(void);    /* Suggested AGP graphics card use                     */
extern void IRQ12(void);    /* Connect PS/2 mouse or set it to other hardware.     */
extern void IRQ13(void);    /* Coprocessor usage                                   */
extern void IRQ14(void);    /* IDE0 Transmission Control Usage                     */
extern void IRQ15(void);    /* IDE1 Transmission Control Usage                     */

#define PORT_KEYDAT 0x60

#define PIT_CTRL    0x43
#define PIT_CNT0    0x40

/* Contains: Initialize Pit chip */
void Initialize_Pit(void);

#endif
