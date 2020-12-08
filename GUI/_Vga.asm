VGA_VBE_MEMORY      equ 0x80070000
VGA_VRAM            equ 0
COLOR               equ 0

[bits 32]

[section .text]
global INITIALIZING_VIDEO_GRAPHICS_ARRAY
global READ_PIXEL
global PUT_PIXEL
global SEND_PIXEL

;[vga/vga.h] void INITIALIZING_VIDEO_GRAPHICS_ARRAY();
INITIALIZING_VIDEO_GRAPHICS_ARRAY:
    xor eax,eax
    mov eax,VGA_VBE_MEMORY+6
    mov eax,[eax]                           ;VRAM
    mov [VGA_VRAM],eax
    ret

;[vga/vga.h] uint32_t READ_PIXEL(int position);
READ_PIXEL:
    mov edi,[esp+4]
    mov eax,[VGA_VRAM]
    add edi,eax
    xor eax,eax
    mov eax,[edi+2]                     ;red
    shl eax,8
    mov eax,[edi+1]                     ;green
    shl eax,8
    mov eax,[edi]                       ;blue
    shl eax,8
    add eax,0xff                        ;255
    ret


;[vga/vga.h] void PUT_PIXEL(int position, byte_t bule, byte_t green, byte_t red);
PUT_PIXEL:
    mov eax,[VGA_VRAM]
    mov edi,[esp+4]
    mov bl,[esp+8]
    add edi,eax
    mov byte [edi],bl                   ;blue
    inc edi
    mov bl,[esp+12]
    mov byte [edi],bl                   ;green
    inc edi
    mov bl,[esp+16]
    mov byte [edi],bl                   ;red

    ret

;[vga/vga.h] void SEND_PIXEL(int position1, int position2);
SEND_PIXEL:
    mov ebx,[esp+4]                     ;position1
    push ebx
    call READ_PIXEL
    add esp,4

    mov edi,[esp+8]                     ;position2
    shr eax,8
    mov edx,eax
    and edx,0xff                        ;blue
    shr eax,8
    mov ecx,eax
    and ecx,0xff                        ;green
    shr eax,8
    mov ebx,eax
    and ebx,0xff                        ;red

    push ebx                            ;red
    push ecx                            ;green
    push edx                            ;blue
    push edi                            ;position2
    call PUT_PIXEL
    add esp,16

    ret
