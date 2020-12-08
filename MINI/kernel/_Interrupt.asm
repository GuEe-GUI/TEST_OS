; kernel/_Interrupt.asm

[bits 32]

extern ISR_Handler
extern IRQ_Handler

; nasm的宏定义
; 有错误号使用空指令
%define ERROR_CODE nop
; 没有错误号就Push无效错误号
%define NO_ERROR_CODE push 0

; %macro 宏函数 参数个数
%macro ISR_CODE 2
; 参数1：%1，参数2：%2，...
[global ISR%1]
ISR%1:
    cli                     ; 关闭中断

    %2                      ; 估计情况决定是否放置Push无效错误号
    push byte %1            ; Push中断向量号

    pusha
    mov ax,ds
    push eax                ; 保存数据段描述符

    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp                ; Registers_S指针
    call ISR_Handler        ; 调用相应中断处理函数这里可以使用，当前仅为测试
                            ; ISR_Handler%1的方法实现对不同中断的不同处理
                            ; 也可以使用[ISR_Handler + %1*4]函数指针数组的表示方法
    add esp,4

    pop ebx                 ; 恢复原来的数据段描述符
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa
    add esp,8           ; 跳过Error_Code
    iret                ; 中断处理函数不能返回，需要使用iret或iretd打断
;宏函数结束
%endmacro

;宏函数名 参数1,参数2,...
ISR_CODE  0,NO_ERROR_CODE
ISR_CODE  1,NO_ERROR_CODE
ISR_CODE  2,NO_ERROR_CODE
ISR_CODE  3,NO_ERROR_CODE
ISR_CODE  4,NO_ERROR_CODE
ISR_CODE  5,NO_ERROR_CODE
ISR_CODE  6,NO_ERROR_CODE
ISR_CODE  7,NO_ERROR_CODE
ISR_CODE  8,ERROR_CODE
ISR_CODE  9,NO_ERROR_CODE
ISR_CODE 10,ERROR_CODE
ISR_CODE 11,ERROR_CODE
ISR_CODE 12,ERROR_CODE
ISR_CODE 13,ERROR_CODE
ISR_CODE 14,ERROR_CODE
ISR_CODE 15,NO_ERROR_CODE
ISR_CODE 16,NO_ERROR_CODE
ISR_CODE 17,ERROR_CODE
ISR_CODE 18,NO_ERROR_CODE
ISR_CODE 19,NO_ERROR_CODE
ISR_CODE 20,NO_ERROR_CODE
ISR_CODE 21,NO_ERROR_CODE
ISR_CODE 22,NO_ERROR_CODE
ISR_CODE 23,NO_ERROR_CODE
ISR_CODE 24,NO_ERROR_CODE
ISR_CODE 25,NO_ERROR_CODE
ISR_CODE 26,NO_ERROR_CODE
ISR_CODE 27,NO_ERROR_CODE
ISR_CODE 28,NO_ERROR_CODE
ISR_CODE 29,NO_ERROR_CODE
ISR_CODE 30,NO_ERROR_CODE
ISR_CODE 31,NO_ERROR_CODE

%macro IRQ 2
[global IRQ%1]
IRQ%1:
    cli
    push byte 0
    push byte %2
    jmp IRQ_COMMON_STUB
%endmacro

IRQ 0,32
IRQ 1,33
IRQ 2,34
IRQ 3,35
IRQ 4,36
IRQ 5,37
IRQ 6,38
IRQ 7,39
IRQ 8,40
IRQ 9,41
IRQ 10,42
IRQ 11,43
IRQ 12,44
IRQ 13,45
IRQ 14,46
IRQ 15,47

IRQ_COMMON_STUB:
    pusha
    mov ax,ds
    push eax

    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp
    call IRQ_Handler
    add esp,4

    pop ebx
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa
    add esp,8
    iret
