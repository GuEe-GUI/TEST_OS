;
;   File:       _Start.asm
;
;   Contains:   Kernel running
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

KERNEL_STACK_TOP equ 0x8009fc00

[bits 32]

extern main         ;OS/_Start.c -> int Start(void)
extern System       ;OS/System.c -> void System(void)
extern POWEROFF     ;OS/Power/PowerMgr.asm -> POWEROFF

[section .data]
global gdt_limit
global gdt_addr

[section .text]
global _START

;Contains: Initialize all segment registeres and start kernel
_START:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp,KERNEL_STACK_TOP

    call main

    call System

    jmp CPU_HLT

;Contains: Stop here
CPU_HLT:
    hlt
    jmp CPU_HLT

;Contains: Poweroff
SHUTDOWN:
    call POWEROFF
    jmp $

GDT_LIMIT:  dw 0
GDT_ADDR:   dd 0
