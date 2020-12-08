;
;   File:       Boot.asm
;
;   Contains:   Boot to Loader
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

LOADER_ADDR         equ 0x7000      ;Loader's address
READ_SECTOR         equ 9           ;Number of sectors to be read
BOOT_START          db  0           ;No. for device startup
BOOT_MESSAGE        db  'Booting'   ;Boot message

[bits 16]

org 0x7c00

;Contains: Boot Start, initialize all segment registers
START:
    mov ax,cs
    mov ds,ax
    mov es,ax
    xor ax,ax
    mov ss,ax
    mov sp,0x7c00

    jmp POINT_BOOT_TITLE

;Contains: Clear screen, output the 'BOOT_MESSAGE' by int 0x10, set cursor's position
POINT_BOOT_TITLE:
    mov ax,0x2              ;Clear screen
    int 0x10

    mov ax,BOOT_MESSAGE
    mov bp,ax
    mov cx,7
    mov ax,0x1300
    mov bx,0xc
    mov dx,0
    int 0x10

    mov ah,0x2              ;Set the cursor position
    mov bh,0
    mov dh,0
    mov dl,7
    int 0x10

    jmp READ_FLOPPY

;Contains: Read the Loader
READ_FLOPPY:
    mov dx,0x0              ;Set dh (head) and dl (drive No., read A disk (0)) to 0 (cylinder, sector, head start position is 1)
    mov cx,0x2              ;Read sector 2 (sector number 1 is boot)
    mov ax,LOADER_ADDR
    mov es,ax
    xor bx,bx
    mov ah,0x2
    mov al,READ_SECTOR
    int 0x13

    jnc ENTER_LOADER        ;carry flag (cf), 0 if successful, 1 if error
    jmp READ_FLOPPY

;Contains: Jump to Loader
ENTER_LOADER:
    jmp LOADER_ADDR:0

times 510-($-$$) db 0
dw 0xaa55
