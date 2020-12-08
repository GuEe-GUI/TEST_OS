KERNEL_STACK_TOP equ 0x8009fc00

[bits 32]

extern os_main

[section .text]
global _START

_START:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp,KERNEL_STACK_TOP

    call os_main

CPU_hlt:
    hlt
    jmp CPU_hlt