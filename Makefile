#
#   File:       Makefile
#
#   Contains:   Compile the OS
#
#   Written by: GUI
#
#   Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
#

#The kernel's virtual entry
KERNEL_ENTRY = 0x80100000

#Load is on the 1 sector of disk, it will use 8 sectors
LOAD_SECTOR_OFFSET  = 1
LOAD_SECTORS        = 8

#Kernel is on the 9 sector,it will use 256 sectors(256 * 512 / 1024 = 128KB)
#The elf file will use memory address 0x10000, and end at  0x42000 (0x10000 + 0x3200)
#Put Loader to the second sector of the disk, occupy 8 sectors (4kb), the kernel starts in the 9th sector
KERNEL_SECTOR_OFFEST = 9
KERNEL_SECTORS       = 348

#Virtual Machine set Memory
MEMORY      = 256

NASM        = nasm
CC          = x86_64-elf-gcc
CP          = g++
LD          = x86_64-elf-ld
DD          = dd
QEMU_I386   = qemu-system-i386
QEMU_X86_64 = qemu-system-x86_64
BOCHS       = bochs
BOCHSDBG    = bochsdbg

BOCHS_BXRC  = _Bochs/bochs.bxrc
BOCHS_LOG   = _Bochs/bochsout.log

#build table
OS_BUILD_OBJ_DIR = _Obj/

#Images dir
IMG_DIR          = _Image/

# %.o:  Represents all .o files
# %.c:  Represents all .c files
# $@:   Indicate the target
# $<:   Indicates the first dependency file
# $^:   Represents all dependent files

#The compile flags
ASM_KERNEL_FLAGS    = -I Inc/ -f elf32
C_CP_KERNEL_FLAGS   = -I ./Inc -c -m32 -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-leading-underscore -ffreestanding
LD_FLAGS            = -m elf_i386 -e _START -Ttext $(KERNEL_ENTRY)

BOOT_BIN    = $(OS_BUILD_OBJ_DIR)Boot.bin
LOADER_BIN  = $(OS_BUILD_OBJ_DIR)Loader.bin
KERNEL_FILE = $(OS_BUILD_OBJ_DIR)OSKernel.bin

GUEEOS_IMG  = GuEeOS.img
HDA_IMG     = $(IMG_DIR)hda.img
HDB_IMG     = $(IMG_DIR)hdb.img

#The objs for the kernel
OBJS = \
	$(OS_BUILD_OBJ_DIR)_Start.o\
	$(OS_BUILD_OBJ_DIR)Start.o\
	$(OS_BUILD_OBJ_DIR)System.o\
	$(OS_BUILD_OBJ_DIR)Display.o\
	$(OS_BUILD_OBJ_DIR)Visual.o\
	$(OS_BUILD_OBJ_DIR)Cga.o\
	$(OS_BUILD_OBJ_DIR)_Cga.o\
	$(OS_BUILD_OBJ_DIR)Memory.o\
	$(OS_BUILD_OBJ_DIR)MemoryMgr.o\
	$(OS_BUILD_OBJ_DIR)Time.o\
	$(OS_BUILD_OBJ_DIR)PowerMgr.o\
	$(OS_BUILD_OBJ_DIR)Broadcast.o\
	$(OS_BUILD_OBJ_DIR)Console.o\
	$(OS_BUILD_OBJ_DIR)IO.o\
	$(OS_BUILD_OBJ_DIR)_Interrupt.o\
	$(OS_BUILD_OBJ_DIR)CMOS.o\
	$(OS_BUILD_OBJ_DIR)Interrupt.o\
	$(OS_BUILD_OBJ_DIR)Timer.o\
	$(OS_BUILD_OBJ_DIR)Shader.o\
	$(OS_BUILD_OBJ_DIR)_Vga.o\
	$(OS_BUILD_OBJ_DIR)Color.o\
	$(OS_BUILD_OBJ_DIR)Font.o\
	$(OS_BUILD_OBJ_DIR)Mouse.o\
	$(OS_BUILD_OBJ_DIR)Render.o\
	$(OS_BUILD_OBJ_DIR)Graphics.o\
	$(OS_BUILD_OBJ_DIR)Filter.o\
	$(OS_BUILD_OBJ_DIR)Vga.o\
	$(OS_BUILD_OBJ_DIR)Window.o\
	$(OS_BUILD_OBJ_DIR)Encoding.o\
	$(OS_BUILD_OBJ_DIR)Math.o\
	$(OS_BUILD_OBJ_DIR)String.o\
	$(OS_BUILD_OBJ_DIR)Text.o\
	$(OS_BUILD_OBJ_DIR)Captain.o\
	$(OS_BUILD_OBJ_DIR)Lockscreen.o\
	$(OS_BUILD_OBJ_DIR)Terminal.o

.PHONY : img

img : OS_LD GuEeOS_Img

OS_LD : $(BOOT_BIN) $(LOADER_BIN) $(KERNEL_FILE)

#outout the img and run
GuEeOS_Img :
	$(DD) if=$(BOOT_BIN) of=$(GUEEOS_IMG) bs=512 count=1 conv=notrunc
	$(DD) if=$(LOADER_BIN) of=$(GUEEOS_IMG) bs=512 seek=$(LOAD_SECTOR_OFFSET) count=$(LOAD_SECTORS) conv=notrunc
	$(DD) if=$(KERNEL_FILE) of=$(GUEEOS_IMG) bs=512 seek=$(KERNEL_SECTOR_OFFEST) count=$(KERNEL_SECTORS) conv=notrunc

#if: Input File
#of: Output File
#bs: Block Size
#seek: Append
#count: (copy time)

floppy :
	-rm $(GUEEOS_IMG)
	$(NASM) $(IMG_DIR)floppy.asm -o $(GUEEOS_IMG)

hardisk :
	-rm $(IMG_DIR)C.img
	$(NASM) $(IMG_DIR)hardisk.asm -o $(IMG_DIR)C.img

qemu :
	$(QEMU_I386) -m $(MEMORY) -fda $(GUEEOS_IMG) -hda $(HDA_IMG) -hdb $(HDB_IMG) -boot a

bochs:
	-rm $(BOCHS_LOG)
	$(BOCHS) -f $(BOCHS_BXRC)

bochsdbg:
	-rm $(BOCHS_LOG)
	$(BOCHSDBG) -f $(BOCHS_BXRC)

run : img qemu

clean :
	-rm $(OBJS) $(OS_BUILD_OBJ_DIR)*.bin

Compile :
#BootLoader Files
$(BOOT_BIN) : Boot/Boot.asm
	$(NASM) -o $@ $<
$(LOADER_BIN) : Boot/Loader.asm
	$(NASM) -o $@ $<

#OS Kernel File
$(KERNEL_FILE) : $(OBJS)
	$(LD) $(LD_FLAGS) -o $(KERNEL_FILE) $(OBJS)

#Asm Files
$(OS_BUILD_OBJ_DIR)%.o : GUI/%.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/%.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Display/Cga/%.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Power/%.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<

#C/CPP Files
$(OS_BUILD_OBJ_DIR)%.o : Apps/__SYSTEM__/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : Apps/Terminal/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : Drivers/Video/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : GUI/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : Library/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Display/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Display/Cga/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Memory/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Server/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<
$(OS_BUILD_OBJ_DIR)%.o : OS/Time/%.c
	$(CC) $(C_CP_KERNEL_FLAGS) -o $@ $<

cmds :
	@echo Makefile cmds:
	@echo img      ---  Make GuEeOS.img
	@echo floppy   ---  Creat an empty floppy disk
	@echo hardisk  ---  Creat an empty hard disk (Can't use by the OS)
	@echo qemu     ---  Start up the GuEeOS.img by qemu
	@echo bochs    ---  Start up the GuEeOS.img by bochs
	@echo bochsdbg ---  Start up the GuEeOS.img by bochs (debug mode)
	@echo run      ---  Make GuEeOS.img and start up the GuEeOS.img by qemu
	@echo clean    ---  Delete the compiled dependency file
	@echo cmds     ---  Get help from Makefile
