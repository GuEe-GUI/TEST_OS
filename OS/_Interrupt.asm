;
;   File:       _InterruptIO.asm
;
;   Contains:   Interrupts controller in assembly
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

[bits 32]

extern Call_ISR
extern IRQ_Handler

[section .text]
global IRQ_COMMON_STUB
global IDT_FLUSH

;Used for interrupts without error codes
%macro ISR_NOERRCODE 1
[global ISR%1]
ISR%1:
    cli                  ;Close interruption
    push 0               ;Invalid push interrupt error codes
    push %1              ;Push interrupt number
    jmp ISR_COMMON_STUB
%endmacro

;Contains: Used for interrupts with error codes
%macro ISR_ERRCODE 1
[global ISR%1]
ISR%1:
    cli                  ;Close interruption
    push %1              ;Push interrupt number
    jmp ISR_COMMON_STUB
%endmacro

;Interrupt Handler: NO.0 ~ 19 are Abnormal interruption of CPU
ISR_NOERRCODE 0          ; 0 #DE Divide by 0 anomaly
ISR_NOERRCODE 1          ; 1 #DB Debug anomaly
ISR_NOERRCODE 2          ; 2 NMI
ISR_NOERRCODE 3          ; 3 BP  Breakpoint Exception
ISR_NOERRCODE 4          ; 4 #OF Overflow
ISR_NOERRCODE 5          ; 5 #BR Cross-border occurs when referring to arrays
ISR_NOERRCODE 6          ; 6 #UD Invalid or undefined opcodes
ISR_NOERRCODE 7          ; 7 #NM Equipment unavailable
ISR_ERRCODE   8          ; 8 #DF Dual failures (Error codes)
ISR_NOERRCODE 9          ; 9 ___ Coprocessor Cross-Section Operations
ISR_ERRCODE   10         ;10 #TS Invalid TSS (Error codes)
ISR_ERRCODE   11         ;11 #NP Segment not present (Error codes)
ISR_ERRCODE   12         ;12 #SS Stack error (Error codes)
ISR_ERRCODE   13         ;13 #GP Conventional protection (Error codes)
ISR_ERRCODE   14         ;14 #PF Page failure (Error codes)
ISR_NOERRCODE 15         ;15 ___ Was Occupied by CPU
ISR_NOERRCODE 16         ;16 #MF Floating Point Processing Unit Error
ISR_ERRCODE   17         ;17 #AC Alignment check
ISR_NOERRCODE 18         ;18 #MC Machine Inspection
ISR_NOERRCODE 19         ;19 #XM SIMD (Single Instruction Multiple Data) Floating point anomaly

;Interrupt Handler: NO.20 ~ 31 are Occupied by Intel
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

;Interrupt Handler: NO.32 ~ 255 are User-defined exceptions
ISR_NOERRCODE 255

;Contains: Interrupt service program
ISR_COMMON_STUB:
    pusha                                   ;pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax,ds
    push eax                                ;Save the data segment descriptor

    mov ax,0x10                             ;Loading the Kernel Data Segment Descriptor
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp                                ;The value of the ESP register at this time is equivalent to the pointer of the SaveRegisters_t structure
    call Call_ISR                           ;OS/Interrupt.c -> void Call_ISR(SaveRegisters_t *regs)
    add esp,4                               ;Clearing the parameters of indentation

    pop ebx                                 ;Restore the original data segment descriptor
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa                                    ;Pops edi, esi, ebp, esp, ebx, edx, ecx, eax
    add esp,8                               ;Cleaning stack error codes and ISR number
    iret                                    ;Stack cs, eip, eflags, ss, esp

;[OS/Interrupt.c] extern void IDT_FLUSH(uint32_t);
;Contains: Flush the idt
IDT_FLUSH:
    mov eax,[esp+4]                         ;Parameters are stored in eax registers
    lidt [eax]                              ;Load to IDTR
    ret

;Contains: Interrupt Request
%macro IRQ 2
[global IRQ%1]
IRQ%1:
    cli
    push byte 0
    push byte %2
    jmp IRQ_COMMON_STUB
%endmacro

IRQ 0,32   ;Computer System Timer
IRQ 1,33   ;keyboard
IRQ 2,34   ;Connecting with IRQ9, MPU-401 MD is used
IRQ 3,35   ;serial device
IRQ 4,36   ;serial device
IRQ 5,37   ;Suggested Sound Card Use
IRQ 6,38   ;Floppy Drive Transmission Control
IRQ 7,39   ;Use of Printer Transfer Control
IRQ 8,40   ;Real-time clock
IRQ 9,41   ;Connect with IRQ2 and can be set to other hardware
IRQ 10,42  ;Suggested Network Card Use
IRQ 11,43  ;Suggested AGP graphics card use
IRQ 12,44  ;Connect PS/2 mouse or set it to other hardware.
IRQ 13,45  ;Coprocessor usage
IRQ 14,46  ;IDE0 Transmission Control Usage
IRQ 15,47  ;IDE1 Transmission Control Usage

;Contains: Interrupt service program for OS
IRQ_COMMON_STUB:
    pusha                                   ;pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax,ds
    push eax                                ;Save the data segment descriptor

    mov ax,0x10                             ;Loading the Kernel Data Segment Descriptor
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp
    call IRQ_Handler                        ;OS/Interrupt.c -> void IRQ_Handler(SaveRegisters_t *regs)
    add esp,4

    pop ebx                                 ;Restore the original data segment descriptor
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa                                    ;Pops edi, esi, ebp, esp, ebx, edx, ecx, eax
    add esp,8                               ;Cleaning stack error codes and ISR number
    iret                                    ;Stack cs, eip, eflags, ss, esp
