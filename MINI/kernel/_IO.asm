; kernel/_IO.asm

[bits 32]

global io_out8, io_out16, io_out32
global io_in8, io_in16, io_in32

;void io_out8(uint32_t port, uint8_t data);
io_out8:
    mov edx,[esp+4] ;port
    mov al,[esp+8]  ;data
    out dx,al
    ret

;void io_out16(uint32_t port, uint16_t data);
io_out16:
    mov edx,[esp+4] ;port
    mov ax,[esp+8]  ;data
    out dx,ax
    ret

;void io_out32(uint32_t port, uint32_t data);
io_out32:
    mov edx,[esp+4] ;port
    mov eax,[esp+8] ;data
    out dx,eax
    ret

;uint8_t io_in8(uint32_t port);
io_in8:
    mov edx,[esp+4] ;port
    mov eax,0
    in al,dx
    ret

;uint16_t io_in16(uint32_t port);
io_in16:
    mov edx,[esp+4] ;port
    mov eax,0
    in ax,dx
    ret

;uint32_t io_in32(uint32_t port);
io_in32:
    mov edx,[esp+4] ;port
    in eax,dx
    ret
