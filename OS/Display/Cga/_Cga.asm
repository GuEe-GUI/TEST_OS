;
;   File:       _Cga.asm
;
;   Contains:   CGA mode interface In Asm
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

;SET_BACKGROUND_COLOR
PALETTE_INDEX_REGISTER  equ 0x3c8       ;Access color register
PALETTE_DATA_REGISTER   equ 0x3c9       ;Read and write red, green, blue
BACKGROUND_INDEX        equ 0           ;Index of background

;PUT_CHAR
CGA_VBE_MEMORY          equ 0x800b8000

[bits 32]

[section .text]
global SET_BACKGROUND_COLOR
global MOVE_CURSOR
global PUT_CHAR
global PUT_STRING
global CLEAR_SCREEN
global ROLL_SCREEN

;[cga.h] void SET_BACKGROUND_COLOR(int red, int green, int blue);
;Contains: Set the CGA mode background color (0~63)
SET_BACKGROUND_COLOR:
    mov dx,PALETTE_INDEX_REGISTER
    mov al,BACKGROUND_INDEX
    out dx,al
    mov dx,PALETTE_DATA_REGISTER

    mov al,[esp+4]          ;Color: red
    out dx,al
    mov al,[esp+8]          ;Color: green
    out dx,al
    mov al,[esp+12]         ;Color: blue
    out dx,al

    ret

;[cga.h] void MOVE_CURSOR(short position);
;Contains: Move the cursor position when output a character
MOVE_CURSOR:
    mov bx,[esp+4]          ;position
    mov dx,0x3d4
    mov al,0x0e
    out dx,al

    mov dx,0x3d5
    mov al,bh
    out dx,al

    mov dx,0x3d4
    mov al,0x0f
    out dx,al

    mov dx,0x3d5
    mov al,bl
    out dx,al

    ret

;[cga.h] void PUT_CHAR(short position, const char character, unsigned char color);
;Contains: Output a character in CGA mode
PUT_CHAR:
    mov eax,[esp+4]                     ;position

    push eax
    call MOVE_CURSOR
    add esp,4
    dec eax

    shl eax,1
    mov edi,eax
    mov al,[esp+8]                      ;character
    mov byte [CGA_VBE_MEMORY+edi],al
    inc edi
    mov al,[esp+12]                     ;color
    mov byte [CGA_VBE_MEMORY+edi],al

    ret

;[cga.h] void CLEAR_SCREEN(short eof);
;Contains: Clear the screen in CGA mode
CLEAR_SCREEN:
    mov ecx,[esp+4]                     ;eof
    mov edi,0

.LOOP_CLEAN_CHAR:
    mov byte [CGA_VBE_MEMORY+edi],0
    inc edi
    mov byte [CGA_VBE_MEMORY+edi],0x0f
    inc edi

    loop .LOOP_CLEAN_CHAR

    push 0
    call MOVE_CURSOR
    add esp,4

    ret

;[cga.h] void ROLL_SCREEN();
;Contains: Roll a line in the screen
ROLL_SCREEN:
    push 1920
    call MOVE_CURSOR
    add esp,4

    mov ecx,2000
    mov edi,0

.LOOP_COPY:
    mov al,byte [CGA_VBE_MEMORY+edi+160]
    mov byte [CGA_VBE_MEMORY+edi],al
    inc edi
    mov al,byte [CGA_VBE_MEMORY+edi+160]
    mov byte [CGA_VBE_MEMORY+edi],al
    inc edi

    loop .LOOP_COPY

CLEAN_LAST_LINE:
    mov ecx,80
    mov edi,3840

.LOOP_CLEAN_LAST_LINE:
    mov byte [CGA_VBE_MEMORY+edi],0
    inc edi
    mov byte [CGA_VBE_MEMORY+edi],0x0f
    inc edi

    loop .LOOP_CLEAN_LAST_LINE

    ret
