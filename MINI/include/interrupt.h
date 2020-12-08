// include/interrupt.h

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <types.h>
#include <io.h>

typedef union Type_S {
    struct {
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // 小端模式x86_64架构常用
            uint16_t Type_Flag:5;   // |0 D 1 1 0| D: Size of gate: 1 = 32 bits; 0 = 16 bits
            uint16_t DPL:2;
            uint16_t P:1;
            uint16_t SetZero:3;
            uint16_t Reserved:5;
        #else // 大端模式 __ORDER_BIG_ENDIAN__
            uint16_t Reserved:5;
            uint16_t SetZero:3;
            uint16_t P:1;
            uint16_t DPL:2;
            uint16_t Type_Flag:5;   // |0 D 1 1 0| D: Size of gate: 1 = 32 bits; 0 = 16 bits
        #endif
    } __attribute__((packed));
    uint16_t All;
} Type_S;

typedef struct IDT_S {
    // High 32bits
    uint16_t Offset0_15;
    uint16_t Segment_Selector;
    // Low 32bits
    Type_S Type;
    uint16_t Offset16_31;
} __attribute__((packed)) IDT_S;

typedef struct IDTR_S {
    uint16_t Limite;
    uint32_t Base;
} __attribute__((packed)) IDTR_S;

#define IDT_BASE 0x00000000
#define IDT_SIZE 0xFF
#define INT_GATE 0x8E00     // 1000 1110 0000 0000

typedef struct Registers_S {
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t Interrupt_Number; // 这里应该是Vector才对
    uint32_t Error_Code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t user_ss;
} __attribute__((packed)) Registers_S;

extern void ISR0(void);
extern void ISR1(void);
extern void ISR2(void);
extern void ISR3(void);
extern void ISR4(void);
extern void ISR5(void);
extern void ISR6(void);
extern void ISR7(void);
extern void ISR8(void);
extern void ISR9(void);
extern void ISR10(void);
extern void ISR11(void);
extern void ISR12(void);
extern void ISR13(void);
extern void ISR14(void);
extern void ISR15(void);
extern void ISR16(void);
extern void ISR17(void);
extern void ISR18(void);
extern void ISR19(void);
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

extern void IRQ0(void);
extern void IRQ1(void);
extern void IRQ2(void);
extern void IRQ3(void);
extern void IRQ4(void);
extern void IRQ5(void);
extern void IRQ6(void);
extern void IRQ7(void);
extern void IRQ8(void);
extern void IRQ9(void);
extern void IRQ10(void);
extern void IRQ11(void);
extern void IRQ12(void);
extern void IRQ13(void);
extern void IRQ14(void);
extern void IRQ15(void);

typedef void (*Interrupt_Handler)(Registers_S*);

void RegisterInterrupt(uint8_t Number, Interrupt_Handler Handler);
void init_IDT(void);

/* 8259A-All */
#define PIC0_ICW1   0x20

/* 8259A-Master */
#define PIC0_OCW2   0x20
#define PIC0_IMR    0x21
#define PIC0_ICW2   0x21
#define PIC0_ICW3   0x21
#define PIC0_ICW4   0x21
#define PIC1_ICW1   0xA0

/* 8259A-Slave */
#define PIC1_OCW2   0xA0
#define PIC1_IMR    0xA1
#define PIC1_ICW2   0xA1
#define PIC1_ICW3   0xA1
#define PIC1_ICW4   0xA1

void init_8259A(void);

#endif // _INTERRUPT_H
