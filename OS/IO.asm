;
;   File:       IO.asm
;
;   Contains:   Operating IO (Input/Output)
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

[bits 32]

[section .text]
global  IO_IN8, IO_IN16, IO_IN32
global  IO_OUT8, IO_OUT16, IO_OUT32
global  IO_LOAD_EFLAGS, IO_STORE_EFLAGS

;[assembly.h] int IO_IN8(int port);
IO_IN8:
    mov edx,[esp+4] ;port
    mov eax,0
    in al,dx
    ret

;[assembly.h] int IO_IN16(int port);
IO_IN16:
    mov edx,[esp+4] ;port
    mov eax,0
    in ax,dx
    ret

;[assembly.h] int IO_IN32(int port);
IO_IN32:
    mov edx,[esp+4] ;port
    in eax,dx
    ret

;[assembly.h] void IO_OUT8(int port, int data);
IO_OUT8:
    mov edx,[esp+4] ;port
    mov al,[esp+8]  ;data
    out dx,al
    ret

;[assembly.h] void IO_OUT16(int port, int data);
IO_OUT16:
    mov edx,[esp+4] ;port
    mov eax,[esp+8] ;data
    out dx,ax
    ret

;[assembly.h] void IO_OUT32(int port, int data);
IO_OUT32:
    mov edx,[esp+4] ;port
    mov eax,[esp+8] ;data
    out dx,eax
    ret

;[assembly.h] int IO_LOAD_EFLAGS();
IO_LOAD_EFLAGS:
    pushfd          ;PUSH EFLAGS
    pop eax
    ret

;[assembly.h] void IO_STORE_EFLAGS(int eflags);
IO_STORE_EFLAGS:
    mov eax,[esp+4]
    push eax
    popfd           ;POP EFLAGS
    ret
