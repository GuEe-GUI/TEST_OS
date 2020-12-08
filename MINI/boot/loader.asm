LOADER_ADDR equ 0x70000
VBEMODE                 equ 0x115             ; 800*600*24Bits模式号
; 以下数字仅表示偏移量
VCOLOR                  equ 0                 ; 颜色位数
SCREENX                 equ 2                 ; 分辨率宽
SCREENY                 equ 4                 ; 分辨率高
VRAM                    equ 6                 ; 显存起始位置

[bits 16]

org 0x70000

START:
	mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0

	;清屏
	mov ax, 0x2
    int 0x10

    jmp ENTER_LOADER

READ_SECTOR equ 9

ENTER_LOADER:
    ;该地址实际是0x10000当前处于实模式
    ;一次只能加载128个扇区，一共384个扇区，因此分3次加载
    mov ax, 0x1000
    mov si, READ_SECTOR
    mov cx, 128
    call load_file

    mov ax,0x2000
    mov si,READ_SECTOR+128
    mov cx,128
    call load_file

    mov ax,0x3000
    mov si,READ_SECTOR+256
    mov cx,128
    call load_file

	;跳过数据段
	jmp Test_0xE820

ARDS times 256 db 0
ARDS_N dw 0

;开始调用0xE820子功能
Test_0xE820:
	;ARDS后续值，第一次调用时置为0
	;[]说明为赋值而不是地址
	mov ebx, [0]
	;"SMAP"
	mov edx, 0x534d4150
	;ARDS 结构缓冲区
	mov di, ARDS

Get_memory_0xE820:
	;子功能号
	mov eax, 0xE820
	;ARDS大小（20字节）
	mov ecx, 20
	int 0x15

	;CF位为1时说明调用出错，为0时说明调用未出错
	jc Test_0xE801

	;增加20字节，指向新的ARDS位置
	add di, cx
	;纪录ARDS个数（inc一般指值（指针）加1）
	inc word [ARDS_N]
	;如果下一个待返回的ADRS的值位0
	;说明已经全部返回（最后一个）
	cmp ebx, 0

	;否则继续获取
	jnz Get_memory_0xE820

	;通过ARDS数量设置循环次数
	mov cx, [ARDS_N]
	mov ebx, ARDS
	;常规清0，最终edx要设置最大内存容量
	mov edx, [0]

.Get_Max_Memory:
	;基地址的低 32 位（x86）
	mov eax, [ebx]
	;内存长度的低 32 位，以字节为单位（x86）
	add eax, [ebx + 8]
	;下一个ARDS结构
	add ebx, 20
	cmp edx, eax

	;eax大于等于edx则跳转
	;直到找出最大内存容量
	jge .Next_ARDS
	mov edx, eax

.Next_ARDS:
	;循环（次数为cx的值）
	loop .Get_Max_Memory
	;这里内存以已经获取
	;可以尝试把他（edx）打印出来
	jmp Get_memory_Complete

Test_0xE801:
	mov ax, 0xE801
	int 0x15

	;CF位为1时说明调用出错，为0时说明调用未出错
	jc Test_0x88

	;算出低15MB的内存（0x400 = 1024）
	;转换单位为byte
	;计算方式有乘法（mul）和右移（shr，可用作除法）
	mov cx, 0x400
	mul cx
	shl edx, 16
	and eax, 0xffff
	or edx, eax
	;加1MB
	add edx, 0x100000

	;算出4GB~16MB内存
	mov eax, [0]
	mov ax, bx
	;64KB
	mov ecx, 0x10000
	mul ecx
	add esi, eax
	mov edx, esi

	jmp Get_memory_Complete

Test_0x88:
	;子功能号：0x88
	mov ah, 0x88
	int 0x15

	;CF位为1时说明调用出错，为0时说明调用未出错
	;这里已经获取内存“彻底”失败了
	jc Get_memory_Fail

	;AX * 1024字节 + 1MB
	and eax, 0xffff
	mov cx, 0x400
	mul cx
	shl edx, 16
	or edx, eax
	add edx, 0x100000

	jmp Get_memory_Complete

Get_memory_Fail:
	mov ax, 0xb800
	mov es, ax
	mov byte [es:14], 'F'
	mov byte [es:15], 0xbf

	;机器挂起，CPU进入休眠状态
	jmp $
	hlt

Get_memory_Complete:
	mov ax, 0xb800
	mov es, ax
	mov byte [es:14], 'M'
	mov byte [es:15], 0x1f

point:
	mov ax, 0xb800
	mov es, ax
	mov cx, 7
	mov di, 0
	mov si, LOAD_MSG

.printf:
	mov al, [si]
	mov byte [es: di], al
	inc si
	inc di
	mov byte [es: di], 0xcf
	inc di
	loop .printf

check_VBE_exists:
    mov ax,0x9000                   ;缓冲区从0x90000开始
    mov es,ax
    mov di,0
    mov ax,0x4f00                   ;检查VBE是否存在
    int 0x10

    cmp ax,0x004f                   ;Ax==0x004f 是否成立，失败则跳转到VBE_fail
    jne VBE_fail

check_VBE_version:
    mov ax,[es:di+4]                ;检查VBE版本，必须是2.0
    cmp ax,0x200
    jb VBE_fail

get_VBE_information:
    mov cx,VBEMODE                  ;模式号
    mov ax,0x4f01                   ;检查模式号
    int 0x10

    cmp ax,0x004f                   ;Ax==0x004f 是否成立，失败则跳转到VBE_fail
    jne VBE_fail

set_VBE:
    mov bx,VBEMODE+0x4000
    mov ax,0x4f02
    int 0x10                        ;这里要是注释掉，依然是文本模式

    mov byte [VCOLOR],8             ;记录图形模式信息
    mov ax,[es:di+0x12]             ;分辨率宽，保存到 0x70002
    mov [SCREENX],ax
    mov ax,[es:di+0x14]             ;分辨率高，保存到 0x70004
    mov [SCREENY],ax
    mov eax,[es:di+0x28]            ;显存起始地址，保存到 0x70006
    mov [VRAM],eax

    jmp load_GDTR

VBE_fail:
    ;机器挂起，CPU进入休眠状态
    jmp $
    hlt

load_GDTR:
	;保存GDT地址到GDTR
	lgdt[gdt]

enable_A20:
    in al,0x92
    or al,10b 		;一般写2
    out 0x92,al

    cli

set_PE:
    mov eax,cr0
    or eax,1
    mov cr0,eax

entry_to_protection_mode:
    jmp dword 0x8:flush

gdt_head:
    dd 0x0000000
    dd 0x0000000

    dw 0x000ffff	;段限制：0-15位
    dw 0x0000000	;段基地址：0-15位
    db 0x0000000	;段基地址：16-23位
    db 10011010b	;段描述符的第6字节属性（代码段可读写）
    db 11001111b	;段描述符的第7字节属性：16-19位
    db 0x0000000	;段描述符的最后一个字节是段基地址的第二部分：24-31位

    dw 0x000ffff	;段限制：0-15位
    dw 0x0000000	;段基地址：0-15位
    db 0x0000000	;段基地址：16-23位
    db 10010010b	;段描述符的第6字节属性（数据段可读写）
    db 11001111b	;段描述符的第7字节属性：limit（位16-19）
    db 0x0000000	;段描述符的最后一个字节是段基地址的第二部分：24-31位

gdt:
    dw gdt - gdt_head - 1
    dd gdt_head

;si: 扇区逻辑区块地址，起点为0
;cx: 扇区数
read_floppy_sector:
    push ax
    push cx
    push dx ;保存缓冲内容
    push bx

    mov ax, si
    xor dx, dx
    mov bx, 18

    div bx
    inc dx
    mov cl, dl
    xor dx, dx
    mov bx, 2

    div bx

    mov dh, dl
    xor dl, dl
    mov ch, al
    pop bx
.rp:
    mov al, 0x01
    mov ah, 0x02
    int 0x13
    jc .rp
    pop dx
    pop cx
    pop ax
    ret

load_file:
    ;段偏移
    mov es, ax
    xor bx, bx
.loop:
    call read_floppy_sector
    add bx, 512
    inc si
    loop .loop
    ret

[bits 32]

flush:
    mov cx,0x10
    mov ds,cx
    mov es,cx
    mov gs,cx
    mov cx,0x10
    mov ss,cx
    mov esp,LOADER_ADDR

	mov dword [0xb8000+160+0],'P'
	mov dword [0xb8000+160+1],0x2f

    jmp Setup_page

Page_Dir_Address equ 0x1000
Page_Table_Address equ 0x2000   ;目录项起始地址
Video_Table_Address equ 0x3000  ;视频模式目录项起始地址

;清理1024字节空间，页目录表项清0
Setup_page:
    mov ecx,1024
    mov ebx,Page_Dir_Address
    xor esi,esi

.SET_ZERO:
    mov byte [ebx+esi],0
    inc esi
    loop .SET_ZERO

    ;一个页目录表大小
    ;4kb = 4 * 1024 bytes = 4096 bytes

    ;‘|’后面填表，其他位不用填，默认为0
    ;7 = 110b -> [PG_P = 1b] [PG_RW = 10b] [PG_US = 100b]
    ;填写页表1（最后2044）
    mov dword [ebx], Page_Table_Address | 0x7
    ;填写页表2
    mov dword [ebx+2048], Page_Table_Address | 0x7
    ;最后一个页目录指向自己的页表
    mov dword [ebx+4092], Page_Dir_Address | 0x7
    ;每个页表可以存放1024个物理页，总4M（1024 * 4kb = 4MB）内存
    mov ecx, 1024
    mov edi,Page_Table_Address
    mov esi, 0x07

.Create_Page:
    mov [edi],esi
    add esi,0x1000
    add edi,4
    loop .Create_Page

    mov eax,[0x70000+VRAM]             ;Map 0xe0000000 to the page directory table
    shr eax,22                         ;Eax = 0xe00
    shl eax,2

    mov edx,Video_Table_Address|0x07
    mov [Page_Dir_Address+eax],edx
    mov edi,Video_Table_Address        ;edi = 0x3000
    mov esi,[0x70000+VRAM]             ;esi = 0xe0000000
    or esi,0x07                        ;esi = 0xe0000007
    mov cx,1024                        ;Map 4M for vram

.mapping_VRAM:
    mov dword [edi],esi
    add edi,4
    add esi,0x1000
    loop .mapping_VRAM                 ;Memory map completion

Entry_to_paging:
    mov eax,Page_Dir_Address
    mov cr3,eax

    ;此处cr0中有PE位（保护模式开关）
    ;因此使用or
    mov eax,cr0
    or eax,1<<31
    mov cr0,eax

point_in_paging_mode:
    ;分页机制下寻址
    mov eax,Page_Dir_Address
    mov ebx,Page_Table_Address
    add eax,ebx
    shl eax,20
    add eax,0xb8000
    mov dword [eax+160+2],'P'
	mov dword [eax+160+3],0x6f
    mov dword [eax+160+4],'a'
	mov dword [eax+160+5],0x6f

    jmp enter_kernel

KERNEL_BIN_BASE_ADDR EQU 0x10000
KERNEL_ENTRY equ 0x80100000

enter_kernel:
    call init_kernel
    ;进入内核
    jmp KERNEL_ENTRY

;这里引用胡同学的注释：
;遍历每一个 Program Header，根据 Program Header 中的信息来确定把什么放进内存，放到什么位置，以及放多少。
init_kernel:
   xor eax,eax
   xor ebx,ebx                          ;记录每一个Program Header Table地址
   xor ecx,ecx                          ;记录每一个Program Header Table数量
   xor edx,edx                          ;记录每一个Program Header Table的大小：e_phentsize

   mov dx,[KERNEL_BIN_BASE_ADDR+42]     ;偏移42字节：e_phentsize
   mov ebx,[KERNEL_BIN_BASE_ADDR+28]    ;偏移28字节：e_phoff，第一个program header偏移量
   add ebx,KERNEL_BIN_BASE_ADDR
   mov cx,[KERNEL_BIN_BASE_ADDR+44]     ;偏移44字节： e_phnum

;遍历每个段
.EACH_SEGMENT:
   cmp byte [ebx+0],0                   ;PT_NULL = 0
   je .PTNULL

   push dword [ebx+16]                  ;p_filesz，memcpy第三个参数: size
   mov eax,[ebx+4]                      ;p_offset, 本段在文件起始的偏移字节
   add eax,KERNEL_BIN_BASE_ADDR         ;本程序段的起始地址
   push eax                             ;memcpy第二个参数： source
   push dword [ebx+8]                   ;memcpy第一个参数： destination
   call memcpy
   add esp,12                           ;memcpy一共3个参数，故3*4=12

.PTNULL:
   add ebx,edx                          ;Edx is the program header size, ie e_phentsize, where ebx points to the next program header
   loop .EACH_SEGMENT

   ret

;逐字节拷贝
memcpy:
   cld
   push ebp
   mov ebp,esp
   push ecx         ;保存ecx内值
   mov edi,[ebp+8]  ;dst
   mov esi,[ebp+12] ;src
   mov ecx,[ebp+16] ;size
   rep movsb        ;逐字节拷贝

   pop ecx
   pop ebp

   ret

LOAD_MSG db 'LOADING'

times 4096-($-$$) db 0