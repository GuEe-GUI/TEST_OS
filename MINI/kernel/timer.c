#include <timer.h>
#include <kernel.h>

void timer_callback(Registers_S *regs) {
    static uint32_t tick = 0;
    printk(KERN_EMERG "Tick: %d\n", tick++);
}

void init_Timer(uint32_t frequency) {
    RegisterInterrupt(IRQ_0, timer_callback);

    // Intel 8253/8254 每秒中断次数
    uint32_t divisor = 1193180 / frequency;

    /*
        0011 0110
        时钟中断由Intel 8253/8254产生，因此不设置为0（8086模式）
        端口地址范围是40h~43h
    */
    io_out8(0x43, 0x36);

    io_out8(0x40, (uint8_t)(divisor & 0xFF));           // 设置低位
    io_out8(0x40, (uint8_t)((divisor >> 8) & 0xFF));    // 设置高位
}