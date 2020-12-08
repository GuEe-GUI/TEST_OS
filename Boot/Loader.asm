;
;   File:       Loader.asm
;
;   Contains:   Load Kernel
;
;   Written by: GUI
;
;   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
;

RES_0                   equ 0x112             ;VBE graphics mode: 640  x  480 x 24 Bits
RES_1                   equ 0x115             ;VBE graphics mode: 800  x  600 x 24 Bits
RES_2                   equ 0x118             ;VBE graphics mode: 1024 x  768 x 24 Bits
RES_3                   equ 0x11B             ;VBE graphics mode: 1280 x 1024 x 24 Bits

VBEMODE                 equ RES_1

DISPLAY_MODE_FLAG       equ 0x500             ;Display Mode's Flag
VCOLOR                  equ 0                 ;Information about the number of colors, The number of bits in the color
SCREENX                 equ 2                 ;Resolution X (screen x), number is just an offset
SCREENY                 equ 4                 ;Resolution Y (screen y), number is just an offset
VRAM                    equ 6                 ;Start address of the image buffer(0xe0000000 or 0x000a0000), number is just an offset

ARDS_SAVE_ADDR          equ 0x501             ;ARDS address of information
ARDS_N                  equ 0                 ;To record the number of ARDS
ARDS_BUF                equ 4

LINE_WIDE               equ 160               ;The wide of a line
VIDEO_MEMORY            equ 0xb800            ;Video memory address
VIDEO_MEMORY_PROTECT    equ 0xb8000           ;Video memory address in protected
VIDEO_MEMORY_PAGE       equ 0x800b8000        ;Video memory address in page
FONT_BACKGROUND_COLOR   equ 0x03              ;Font and background color

PAGE_DIR_ADDR           equ 0x1000            ;Page directory physical address
PAGE_TBL_ADDR           equ 0x2000            ;Directory entry start address
VIDEO_PAGE_TBL_ADDR     equ 0x3000            ;Video Directory entry start address

KERNEL_START_SECTOR     equ 0x9               ;Kernel is on the 9th sectors
KERNEL_SECTORS          equ 348               ;Now kernel only use 256 sector(256*512/1024 = 128KB)
KERNEL_BIN_BASE_ADDR    equ 0x10000           ;Kernel's address
VIR_KERNEL_ENTRY        equ 0x80100000        ;Kernel stack top

[bits 16]

org 0x70000

jmp START

;Creat a FAt12 File System
;Boot_Name_12            db  'GuEesFAT'      ;The name of the boot area (8 bytes)
;Bytes_Per_Sector_12     dw  512             ;The size of each sector (must be 512 bytes)
;Sectors_Per_Cluster_12  db  1               ;The size of the cluster (must be 1 sector)
;Boot_Sector_12          dw  1               ;The starting position of the FAT (generally starting from the first sector)
;FATs_Number_12          db  2               ;Number of FATs (must be 2)
;Root_Dir_Size_12        dw  224             ;The size of the root directory (usually set to 224 items)
;Disk_Size_12            dw  2880            ;The size of the disk (must be 2880 sectors)
;Disk_Type_12            db  0xf0            ;Type of disk (must be 0xf0)
;FAT_Size_12             dw  9               ;Length of FAT (must be 9 sectors)
;Sectors_Per_Track_12    dw  18              ;1 track has several sectors (must be 18)
;Heads_Number_12         dw  2               ;Number of heads (must be 2)
;Hidden_Sectors_12       dd  0               ;Do not use partition, must be 0
;TotSectors32_12         dd  0               ;If wTotalSectorCount is 0, the number of sectors is recorded by this value
;BS_DrvNum_12            db  0               ;Interrupt 13 drive letter
;Reserved_12             db  0               ;Unused
;Drive_Signature_12      db  29h             ;Extended boot tag (29h)
;Volume_ID_12            dd  0               ;Volume serial number
;Disk_Name_12            db  'GuEesDisk  '   ;The name of the disk (11 bytes)
;File_System_12          db  'FAT12   '      ;Disk format name (8 bytes)

;Creat a FAt32 File System
Boot_Name_32            db  'GuEesFAT'
Bytes_Per_Sector_32     dw  0x200
Sectors_Per_Cluster_32  db  8
Boot_Sector_32          dw  20h
FATs_Number_32          db  2
Root_Dir_Size_32        dw  0
Disk_Size_32            dw  0
Disk_Type_32            db  0F8h
FAT_Size_32             dw  0
Sectors_Per_Track_32    dw  63
Heads_Number_32         dw  255
Hidden_Sectors_32       dd  63
TotSectors32_32         db  0xbf,0x64,0x9c,0x00
BS_DrvNum_32            db  0x10,0x27,0x00,0x00
Ext_Flags               dw  0
FS_Version              dw  0
Root_Dir_Strt_Clus      dd  2
FS_Info_Sec             dw  0x01
Bk_Up_Boot_Sec          dw  0x06
Reserved                dw  0,0,0,0,0,0
Drive_Signature_32      db  80h
HeadTemp                db  0
Signature               db  29h
SerialNumber            db  0x07,0x16,0x1a,0x39
Disk_Name_32            db  'GuEesDisk  '
File_System_32          db  'FAT32   '

;Contains: Loader Start, initialize all segment registers
START:
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0

    mov si,BOOT_OK_MSG
    mov cx,12
    mov di,0
    call POINT_MESSAGE_IN16BIT

    mov dx,11
    call SET_CUR_IN_REAL

    jmp LOAD_KERNEL

;Contains: Loader Kernel
LOAD_KERNEL:
    mov ax,0x1000
    mov si,KERNEL_START_SECTOR
    mov cx,128
    call LOAD_FILE

    mov ax,0x2000
    mov si,KERNEL_START_SECTOR+128
    mov cx,128
    call LOAD_FILE

    mov ax,0x3000
    mov si,KERNEL_START_SECTOR+256
    mov cx,128
    call LOAD_FILE

    mov ax,0x5000                       ;Load File
    mov si,400
    mov cx,25
    call LOAD_FILE

    jmp GET_MEMORY

;Contains: Get the memory by 0xe820
GET_MEMORY:
    xor ebx,ebx
    mov edx,0x534d4150
    mov di,ARDS_BUF
    mov ax,ARDS_SAVE_ADDR               ;ARDS_SAVE_ADDR = 0x501, Paging Mode: 0x80005010
    mov es,ax
    mov word [es:ARDS_N],0

LOOP_GET_MEMORY_0XE820:
    mov eax,0xe820
    mov ecx,20
    int 0x15

    jc GET_MEMORY_FAIL                  ;cf = 0 --> call is right, cf = 1 --> call is wrong
    add di,cx                           ;Add 20 bytes, point the new ARDS address
    inc word [es:ARDS_N]                ;Add ARDS number

    cmp ebx,0                           ;whether to the last (ebx = 0)
    jnz LOOP_GET_MEMORY_0XE820

GET_MEMORY_COMPLETE:
    mov si,MEMORY_OK_MSG
    mov cx,18
    mov di,LINE_WIDE
    call POINT_MESSAGE_IN16BIT

    mov dx,(LINE_WIDE>>1)+17
    call SET_CUR_IN_REAL

    jmp DISPLAY_MODE_SET                ;This code provides mode switching (Debug Mode)
    jmp DEFAULT_DISPLAY

GET_MEMORY_FAIL:
    mov si,MEMORY_ERROR_MSG
    mov cx,19
    mov di,LINE_WIDE
    call POINT_MESSAGE_IN16BIT

    mov dx,(LINE_WIDE>>1)+18
    call SET_CUR_IN_REAL

    jmp $
    hlt

;Contains: Set the display mode, get the input by int 0x16
DISPLAY_MODE_SET:
    mov si,DISPLAY_MODE_MSG
    mov cx,21
    mov di,LINE_WIDE*2
    call POINT_MESSAGE_IN16BIT

    mov dh,2
    mov dl,21
    call SET_CUR_IN_REAL

    mov dx,2                            ;dx = 2 is VGA Mode, dx = 1 is CGA Mode
    mov si,DISPLAY_VGA_MODE_MSG
    call DISPLAY_MODE_LI

GET_INPUT:
    mov ah,0
    int 0x16

    cmp ah,0x1c                         ;0x1c is the enter's keycode is BIOS
    je ENTER_DISPLAY_MODE

    cmp dx,2
    je DISPLAY_MODE_LI_CGA              ;Enter to CGA Mode Title

    call DISPLAY_MODE_LI_VGA            ;Enter to VGA Mode Title

    jmp GET_INPUT                       ;Retrieve input

DISPLAY_MODE_LI_VGA:
    mov si,DISPLAY_VGA_MODE_MSG
    call DISPLAY_MODE_LI

    mov dx,2

    jmp GET_INPUT

DISPLAY_MODE_LI_CGA:
    mov si,DISPLAY_CGA_MODE_MSG
    call DISPLAY_MODE_LI

    mov dx,1

    jmp GET_INPUT

ENTER_DISPLAY_MODE:
    mov ax,DISPLAY_MODE_FLAG            ;DISPLAY_MODE_FLAG = 0x500, Paging Mode: 0x80005000
    mov es,ax
    mov byte [es:0],1                   ;Ask the Graphics/Video.c InitVram() is CGA Mode
    cmp dx,1
    je BIOS_KEYSTATUS

    mov byte [es:0],2                   ;Ask the Graphics/Video.c InitVram() is VGA Mode
    jmp CHECK_VBE_EXISTS

DEFAULT_DISPLAY:
    mov dx,2
    jmp ENTER_DISPLAY_MODE

DISPLAY_MODE_LI:
    mov cx,8
    mov di,LINE_WIDE*2+42
    call POINT_MESSAGE_IN16BIT

    ret

;Contains: Check the VBE, set the resolution, save the information of VGA
CHECK_VBE_EXISTS:
    mov ax,0x9000                   ;Check if VBE exists, buffer starts from 0x90000
    mov es,ax
    mov di,0
    mov ax,0x4f00                   ;Check the VBE presence function, specify ax=0x4f00
    int 0x10

    cmp ax,0x004f                   ;Ax=0x004f exists
    jne VBE_SET_FAIL

CHECK_VBE_VERSION:
    mov ax,[es:di+4]                ;Check the VBE version, it must be VBE 2.0 and above
    cmp ax,0x200
    jb VBE_SET_FAIL

GET_VBE_INFORMATION:
    mov cx,VBEMODE                  ;Mode number, vbe's information: 256 bytes
    mov ax,0x4f01                   ;Get the VGA mode function, specify ax=0x4f01
    int 0x10

    cmp ax,0x004f                   ;Ax=0x004f specifies that this mode can be used
    jne VBE_SET_FAIL

;[...] is the default data segment ds, ds=0x7000 (0x70000 is the Loader start address)
;[es:di+0x19] Number of colors 8 bits, 16 bits, 24 bits, 32 bits
;[es:di+0x1b] The way of specify the color palette, etc.
;[es:di+0x00] Mode attribute bit7 is 1 and can be added to 0x4000 for easy operation.
;Get the VGA mode memory address at 0x70000~0x70006

SET_RESOLUTION:
    mov bx,VBEMODE+0x4000
    mov ax,0x4f02
    int 0x10                        ;Comment this paragraph will into CGA mode

    mov byte [VCOLOR],8             ;Record VGA mode information
    mov ax,[es:di+0x12]             ;[es:di+0x12] Resolution X [SCREENX] is saved at 0x70002
    mov [SCREENX],ax
    mov ax,[es:di+0x14]             ;[es:di+0x14] Resolution Y [SCREENY] is saved at 0x70004
    mov [SCREENY],ax
    mov eax,[es:di+0x28]            ;[es:di+0x28] VRAM address [VRAM] is saved at 0x70006
    mov [VRAM],eax

    jmp BIOS_KEYSTATUS

VBE_SET_FAIL:
    mov si,VGA_MODE_FAIL_MSG
    mov cx,5
    mov di,LINE_WIDE*2+60
    call POINT_MESSAGE_IN16BIT

;Contains: Get the status of various LED by BIOS
BIOS_KEYSTATUS:
    mov ah,0x02
    int 0x16                        ;Use the BIOS to get the status of various LED indicators on the keyboard (keyboard BIOS)

    jmp LOAD_GDT_AND_IDT

;Contains: Point the string in 16bits mode
POINT_MESSAGE_IN16BIT:
    mov ax,VIDEO_MEMORY
    mov es,ax

.POINT_STRING_IN16BIT:
    mov al,[si]                     ;Input the strings
    mov byte [es:di],al             ;Print the current character
    inc si                          ;Point to the next character
    inc di                          ;Point to the next position
    mov byte [es:di],0x0c           ;Set the current character color and background color
    inc di                          ;Point to the next position
    loop .POINT_STRING_IN16BIT

    ret

;Contains: Set cursor's position in 16 bits mode
SET_CUR_IN_REAL:
    mov ah,0x2                      ;Set the cursor position
    mov bh,0
    int 0x10

    ret

;Contains: Load gdt and idt, enable A20, set the bit of CR0's PE, enter protected mode
LOAD_GDT_AND_IDT:
    lgdt[GDT_DESCRIPTOR]            ;Save the gdt address to gdtr
    lidt[IDT_DESCRIPTOR]            ;Save the idt address to lidt

ENABLE_A20GATE:
    in al,0x92
    or al,2
    out 0x92,al

    cli                             ;Clear Interupt (sti: Set Interupt)

SET_BIT_OF_PE:
    mov eax,cr0
    or eax,1
    mov cr0,eax

ENTER_PROTECTED_MODE:
    jmp dword CODE_ADDRESS:FLUSH    ;CODE_ADDRESS = 0x8

;1kb alignment, to get integers for gdt address easier
times 1024-($-$$) db 0

;The first item of GDT must be 0 (CPU requirement)
GDT_START:

GDT_NULL:
    dd 0                    ;dd, 4 bits
    dd 0

;Code segment global descriptor
GDT_CODE:
    dw 0xffff               ;Segment limit: Limit (bits 0-15)
    dw 0                    ;Segment base address: Base (bits 0-15)
    db 0                    ;Segment base address: Base (bits 16-23)
    db 10011010b            ;The 6th byte attribute of the segment descriptor (the code segment is readable and writable), 0x9a=10011010
    db 11001111b            ;The 7th byte attribute of the segment descriptor: Limit(bits 16-19), 0xcf=11001111
    db 0                    ;The last byte of the segment descriptor is the second part of the segment base address: Base(bits 24-31)

;Data segment global descriptor
GDT_DATA:
    dw 0xffff               ;Segment limit: Limit (bits 0-15)
    dw 0                    ;Segment base address: Base (bits 0-15)
    db 0                    ;Segment base address: Base (bits 16-23)
    db 10010010b            ;The 6th byte attribute of the segment descriptor (data segment is readable and writable), 0x92=10010010
    db 11001111b            ;The 7th byte attribute of the segment descriptor: Limit(bits 16-19), 0xcf=11001111
    db 0                    ;The last byte of the segment descriptor is the second part of the segment base address: Base(bits 24-31)

GDT_END:

;GDT descriptior
GDT_DESCRIPTOR:
    dw GDT_END-GDT_START-1  ;The size of the GDT (from 0 to 1)
    dd GDT_START            ;GDT start address

;IDT descriptior
IDT_DESCRIPTOR:
    dw 0
    dw 0
    dw 0

;Descriptor address
CODE_ADDRESS equ GDT_CODE-GDT_START
DATA_ADDRESS equ GDT_DATA-GDT_START

;Contains: Load floppy, Copy data
READ_FLOPPY_SECTOR:
    push ax
    push cx
    push dx
    push bx

    mov ax,si
    xor dx,dx
    mov bx,18

    div bx
    inc dx
    mov cl,dl
    xor dx,dx
    mov bx,2
    div bx

    mov dh,dl
    xor dl,dl
    mov ch,al
    pop bx

.REPEATED_PASTING:
    mov al,0x01
    mov ah,0x02
    int 0x13

    jc .REPEATED_PASTING
    pop dx
    pop cx
    pop ax

    ret

;Ax = segment offset written
;Si = sector LBA address
;Cx = number of sectors

LOAD_FILE:
    mov es,ax
    xor bx,bx

.LOOP:
    call READ_FLOPPY_SECTOR
    add bx,512
    inc si
    loop .LOOP

    ret

[bits 32]

;Contains: Initialize some registers, point 'PROTECTED_MODE_OK_MSG'
FLUSH:
    mov cx,0x10                         ;Load data segment selector
    mov ds,cx
    mov es,cx
    mov gs,cx
    mov cx,0x10                         ;Load stack segment selector
    mov ss,cx
    mov esp,0x70000                     ;Save stack pointer, 0x70000 is the Loader start address

    mov esi,PROTECTED_MODE_OK_MSG
    mov ecx,22
    mov edi,0

.POINT_PROTECT_OK:
    mov eax,[esi]
    mov dword [VIDEO_MEMORY_PROTECT+LINE_WIDE*3+edi],eax
    inc esi
    inc edi
    mov dword [VIDEO_MEMORY_PROTECT+LINE_WIDE*3+edi],FONT_BACKGROUND_COLOR
    inc edi
    loop .POINT_PROTECT_OK

    mov bx,3*(LINE_WIDE>>1)+21
    call SET_CUR_IN_32BITS

;Contains: Startup paging, create pages, mapping vram, save the pages address to CR3, set the bit of CR0's PG
START_TO_PAGING_MODE:
    mov ecx,1024
    mov ebx,PAGE_DIR_ADDR
    xor esi,esi

CLEAN_PDT:
    mov byte [ebx+esi],0
    inc esi
    loop CLEAN_PDT

    mov edi,PAGE_TBL_ADDR
    mov ebx,PAGE_DIR_ADDR

    mov dword [ebx],PAGE_TBL_ADDR|0x7
    mov dword [ebx+512*4],PAGE_TBL_ADDR|0x7
    mov dword [ebx+4092],PAGE_DIR_ADDR|0x7

    mov cx,1024
    mov esi,0|0x7

CREATE_PAGE:
    mov [edi],esi
    add esi,0x1000
    add edi,4
    loop CREATE_PAGE

    mov eax,[0x70000+VRAM]             ;Map 0xe0000000 to the page directory table
    shr eax,22                         ;Eax = 0xe00
    shl eax,2

    mov edx,VIDEO_PAGE_TBL_ADDR|0x07
    mov [PAGE_DIR_ADDR+eax],edx
    mov edi,VIDEO_PAGE_TBL_ADDR        ;edi = 0x3000
    mov esi,[0x70000+VRAM]             ;esi = 0xe0000000
    or esi,0x07                        ;esi = 0xe0000007
    mov cx,1024                        ;Map 4M for vram

.MAPPING_VRAM:
    mov dword [edi],esi
    add edi,4
    add esi,0x1000
    loop .MAPPING_VRAM                 ;Memory map completion

SAVE_THE_ADDRESS_TO_CR3:
    mov eax,PAGE_DIR_ADDR
    mov cr3,eax

SET_BIT_OF_PG:
    mov eax,cr0
    or eax,1<<31                       ;PG = 0x80000000
    mov cr0,eax

ENTER_PAGING_MODE_COMPLETE:
    mov esi,PAGING_OK_MSG
    mov ecx,19
    mov edi,4*LINE_WIDE
    call POINT_STRING_IN32BIT_PAGE

    mov bx,4*(LINE_WIDE>>1)+18
    call SET_CUR_IN_32BITS

;Contains: Start to into the kernel
CALL_KERNEL:
    mov esi,LOADING_SYSTEM_MSG
    mov ecx,18
    mov edi,5*LINE_WIDE
    call POINT_STRING_IN32BIT_PAGE

    mov bx,5*(LINE_WIDE>>1)+17
    call SET_CUR_IN_32BITS

    call INIT_KERNEL

    call CLEAR_SCREEN

    mov bx,0
    call SET_CUR_IN_32BITS

    jmp VIR_KERNEL_ENTRY                ;Into the Kernel (End of Loader)

;Contains: Copy the segment in Kernel.bin to the compiled address
INIT_KERNEL:
   xor eax,eax
   xor ebx,ebx                          ;Ebx logger header table address
   xor ecx,ecx                          ;Cx record the number of program headers in the program header table
   xor edx,edx                          ;Dx record program header size, ie e_phentsize

   mov dx,[KERNEL_BIN_BASE_ADDR+42]     ;The attribute at offset 42 bytes is e_phentsize, indicating the size of the program header
   mov ebx,[KERNEL_BIN_BASE_ADDR+28]    ;The 28 bytes of the beginning part of the offset file is e_phoff, indicating the offset of the first program header in the file
   add ebx,KERNEL_BIN_BASE_ADDR
   mov cx,[KERNEL_BIN_BASE_ADDR+44]     ;The 44 bytes of the beginning part of the offset file is e_phnum, indicating that there are several program headers

.EACH_SEGMENT:
   cmp byte [ebx+0],0                   ;PT_NULL
   je .PTNULL

   push dword [ebx+16]                  ;The offset of 16 bytes in the program header is p_filesz, the third parameter of the function memcpy: size
   mov eax,[ebx+4]                      ;The position offset from the program header by 4 bytes is p_offset
   add eax,KERNEL_BIN_BASE_ADDR         ;Plus the physical address to which kernel.bin is loaded, eax is the physical address of the segment
   push eax                             ;Push the second parameter of the function memcpy: source address
   push dword [ebx+8]                   ;Push the first parameter of the function memcpy: destination address, the position of the first 8 bytes of the offset program is p_vaddr, which is the destination address
   call MEMORY_COPY                     ;Call mem_cpy to complete the segment copy
   add esp,12                           ;Clean up the three parameters pushed in the stack

.PTNULL:
   add ebx,edx                          ;Edx is the program header size, ie e_phentsize, where ebx points to the next program header
   loop .EACH_SEGMENT

   ret

MEMORY_COPY:
   cld
   push ebp
   mov ebp,esp
   push ecx                             ;The rep command uses ecx, but ecx is still useful for the loop of the outer segment, so it is first pushed into the stack
   mov edi,[ebp+8]                      ;dst
   mov esi,[ebp+12]                     ;src
   mov ecx,[ebp+16]                     ;size
   rep movsb                            ;Byte-by-byte copy

   pop ecx                              ;Return
   pop ebp

   ret

;Contains: Clear the screen in protected mode
CLEAR_SCREEN:
    mov eax,VIDEO_MEMORY_PAGE           ;Clear the point's message
    mov edi,eax
    mov ecx,2000                        ;Length of all line = 80 * 25

.CLEAR_POINT:
    mov dword [edi+0],0
    add edi,1
    mov dword [edi+0],0xf
    add edi,1
    loop .CLEAR_POINT

    ret

;Contains: Point in paging
POINT_STRING_IN32BIT_PAGE:
    mov eax,[esi]
    mov dword [VIDEO_MEMORY_PAGE+edi],eax
    inc esi
    inc edi
    mov dword [VIDEO_MEMORY_PAGE+edi],FONT_BACKGROUND_COLOR
    inc edi
    loop POINT_STRING_IN32BIT_PAGE

    ret

;Contains: Set cursor's position in 32 bits mode
SET_CUR_IN_32BITS:
    mov dx,0x3d4
    mov al,0x0e
    out dx,al

    mov dx,0x3d5
    mov al,bh                           ;bh = 0 (screen Y)
    out dx,al

    mov dx,0x3d4
    mov al,0x0f
    out dx,al

    mov dx,0x3d5
    mov al,bl                           ;bl = 0 (screen X)
    out dx,al

    ret

;Point message
BOOT_OK_MSG             db  '[ OK ] Boot '              ;String Length: 12
MEMORY_OK_MSG           db  '[ OK ] Get Memory '        ;String Length: 18
MEMORY_ERROR_MSG        db  'Get Memory Failed! '       ;String Length: 19
DISPLAY_MODE_MSG        db  'Select Display Mode: '     ;String Length: 21
DISPLAY_VGA_MODE_MSG    db  'VGA Mode'                  ;String Length:  8
DISPLAY_CGA_MODE_MSG    db  'CGA Mode'                  ;String Length:  8
VGA_MODE_FAIL_MSG       db  'Fail!'                     ;String Length:  5
PROTECTED_MODE_OK_MSG   db  '[ OK ] Protected Mode '    ;String Length: 22
PAGING_OK_MSG           db  '[ OK ] Paging Mode '       ;String Length: 19
LOADING_SYSTEM_MSG      db  'Loading System... '        ;String Length: 18

times 4096-($-$$) db 0
