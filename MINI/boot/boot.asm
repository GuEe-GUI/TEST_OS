LOADER_ADDR         equ 0x7000
READ_SECTOR         equ 9
READ_SECTOR_OFFSET  equ 1
BOOT_START          db  0
org 0x7c00

START:
    mov ax,cs
    mov ds,ax
    mov es,ax
    xor ax,ax
    mov ss,ax
    mov sp,0x7c00
    mov ax,0x2              ;Clear screen
    int 0x10

READ_FLOPPY:
    mov dx,0x0              ;Set dh (head) and dl (drive number, read A disk (0)) to 0 (cylinder, sector, head start position is 1)
    mov cx,0x2              ;Read sector 2 (sector number 1 is boot)
    mov ax,LOADER_ADDR
    mov es,ax
    xor bx,bx
    mov ah,0x2
    mov al,READ_SECTOR
    int 0x13

    jnc ENTER_LOADER        ;carry flag (cf), 0 if successful, 1 if error
    jmp READ_FLOPPY

ENTER_LOADER:
    jmp LOADER_ADDR:0

times 510-($-$$) db 0
dw 0xaa55