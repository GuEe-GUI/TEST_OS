;
;   File:       PowerMgr.asm
;
;   Contains:   Power Manager
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

SPValueInRealMode   dw  0

[bits 32]

extern IO_OUT16                     ;OS/IO.asm -> IO_OUT16

[section .text]
global POWEROFF

;[assembly.h] void POWEROFF();
;Contains: poweroff
POWEROFF:
    call SHUTDOWN_IN_VM             ;Shutdwon in Virtual Machine

    call TRY_IO_TO_SHUTDOWN         ;Poweroff in protection mode

    jmp SHUTDOWN_IN_REAL_MODE      ;Start to poweroff in real mode

SHUTDOWN_IN_VM:
    push 0x2000                     ;Qemu (newer) 
    push 0x604
    call IO_OUT16
    add esp,4*2

    push 0x2000                     ;Bochs and Qemu (than 2.0) 
    push 0xb004
    call IO_OUT16
    add esp,4*2

    push 0x3400                     ;Virtualbox
    push 0x4004
    call IO_OUT16
    add esp,4*2

    ret

TRY_IO_TO_SHUTDOWN:
    cli
    mov edx,0x00008900
    mov al,0x53
    out dx,al
    mov al,0x68
    out dx,al
    mov al,0x75
    out dx,al
    mov al,0x74
    out dx,al
    mov al,0x64
    out dx,al
    mov al,0x6f
    out dx,al
    mov al,0x77
    out dx,al
    mov al,0x6e
    out dx,al

    ret

SHUTDOWN_IN_REAL_MODE:
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax

    mov sp,[SPValueInRealMode]

SET_BIT_OF_PE:
    mov eax,cr0
    and al,0xfe
    mov cr0,eax

CLOSE_A20:
    in al,0x92
    and al,0xfe
    out 0x92,al

    sti                     ;Open Interrupt

ENTER_REAL_MODE:
    jmp 0:SHUTDOWN

[bits 16]

SHUTDOWN:
    mov ax,0x1000
    mov ax,ss
    mov sp,0xf000
    mov ax,0x5307            ;Advanced Power Management Function
    mov bx,0x0001            ;Device ID (1): All the devices
    mov cx,0x0003            ;PowerOff (Shutdown)
    int 0x15

    jmp $
