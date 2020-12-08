// kernel/interrupt.c

#include <interrupt.h>
#include <string.h>
#include <kernel.h>

IDT_S IDT[256];
IDTR_S IDTR;
Interrupt_Handler InterruptHandlers[256] = {NULL};

static void init_IDT_Descriptor(uint16_t Segment_Selector, uint32_t Offset, uint16_t Type, IDT_S *IDT) {
    IDT->Offset0_15 = Offset & 0xffff;
    IDT->Segment_Selector = Segment_Selector;
    IDT->Type.All = Type;
    IDT->Offset16_31 = (Offset & 0xffff0000) >> 16;
    return;
}

void RegisterInterrupt(uint8_t Number, Interrupt_Handler Handler) {
    InterruptHandlers[Number] = Handler;

    return;
}

void ISR_Handler(Registers_S *Registers) {
    if (InterruptHandlers[Registers->Interrupt_Number] != NULL) {
        InterruptHandlers[Registers->Interrupt_Number](Registers);
    } else {
        printk(KERN_EMERG"InterruptNumber: %d\n", Registers->Interrupt_Number);
    }
}

void IRQ_Handler(Registers_S *Registers) {
    // 发送重置信号
    if (Registers->Interrupt_Number >= 40) {
        // 主
        io_out8(PIC1_ICW1, 0x20);
    }
    // 从
    io_out8(PIC0_OCW2, 0x20);

    if (InterruptHandlers[Registers->Interrupt_Number] != NULL) {
        InterruptHandlers[Registers->Interrupt_Number](Registers);
    }
}

void init_IDT(void) {
    memset((uint8_t*)&InterruptHandlers, 0, sizeof(Interrupt_Handler) << 8);

    IDTR.Limite = (sizeof(IDT_S) << 8) - 1;
    IDTR.Base  = (uint32_t)&IDT;

    memset((uint8_t*)&IDT, 0, sizeof(IDT_S) << 8);

    init_IDT_Descriptor(0x08, (uint32_t)ISR0,  INT_GATE, &IDT[0]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR1,  INT_GATE, &IDT[1]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR2,  INT_GATE, &IDT[2]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR3,  INT_GATE, &IDT[3]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR4,  INT_GATE, &IDT[4]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR5,  INT_GATE, &IDT[5]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR6,  INT_GATE, &IDT[6]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR7,  INT_GATE, &IDT[7]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR8,  INT_GATE, &IDT[8]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR9,  INT_GATE, &IDT[9]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR10, INT_GATE, &IDT[10]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR11, INT_GATE, &IDT[11]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR12, INT_GATE, &IDT[12]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR13, INT_GATE, &IDT[13]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR14, INT_GATE, &IDT[14]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR15, INT_GATE, &IDT[15]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR16, INT_GATE, &IDT[16]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR17, INT_GATE, &IDT[17]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR18, INT_GATE, &IDT[18]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR19, INT_GATE, &IDT[19]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR20, INT_GATE, &IDT[20]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR21, INT_GATE, &IDT[21]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR22, INT_GATE, &IDT[22]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR23, INT_GATE, &IDT[23]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR24, INT_GATE, &IDT[24]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR25, INT_GATE, &IDT[25]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR26, INT_GATE, &IDT[26]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR27, INT_GATE, &IDT[27]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR28, INT_GATE, &IDT[28]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR29, INT_GATE, &IDT[29]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR30, INT_GATE, &IDT[30]);
    init_IDT_Descriptor(0x08, (uint32_t)ISR31, INT_GATE, &IDT[31]);

    init_IDT_Descriptor(0x08, (uint32_t)IRQ0,  INT_GATE, &IDT[32]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ1,  INT_GATE, &IDT[33]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ2,  INT_GATE, &IDT[34]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ3,  INT_GATE, &IDT[35]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ4,  INT_GATE, &IDT[36]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ5,  INT_GATE, &IDT[37]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ6,  INT_GATE, &IDT[38]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ7,  INT_GATE, &IDT[39]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ8,  INT_GATE, &IDT[40]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ9,  INT_GATE, &IDT[41]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ10, INT_GATE, &IDT[42]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ11, INT_GATE, &IDT[43]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ12, INT_GATE, &IDT[44]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ13, INT_GATE, &IDT[45]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ14, INT_GATE, &IDT[46]);
    init_IDT_Descriptor(0x08, (uint32_t)IRQ15, INT_GATE, &IDT[47]);

    //
    // 加载IDTR
    //
    __asm__ ("lidtl (IDTR)");
}

void init_8259A(void) {
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);

    io_out8(PIC0_ICW1, 0x11); // 0001 0001
    io_out8(PIC0_ICW2, 0x20); // 0010 0000
    io_out8(PIC0_ICW3, 0x04); // 0000 0100
    io_out8(PIC0_ICW4, 0x01); // 0000 0001

    io_out8(PIC1_ICW1, 0x11); // 0001 0001
    io_out8(PIC1_ICW2, 0x28); // 0010 1000
    io_out8(PIC1_ICW3, 0x02); // 0000 0010
    io_out8(PIC1_ICW4, 0x01); // 0000 0001

    io_out8(PIC0_IMR, 0xfe);
    io_out8(PIC1_IMR, 0xff);
}
