// kernel/io.c​
#include <io.h>

void NMI_enable() {
    io_out8(0x70, io_in8(0x70) & 0x7F);
}

void NMI_disable() {
    io_out8(0x70, io_in8(0x70) | 0x80);
}
