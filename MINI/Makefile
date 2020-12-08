LOAD_SECTOR_OFFSET  = 1
LOAD_SECTORS        = 8

KERNEL_SECTOR_OFFEST = 9
KERNEL_SECTORS       = 348

NASM = nasm
CC   = x86_64-elf-gcc
LD   = x86_64-elf-ld
DD   = dd
QEMU = qemu-system-i386

BUILD_DIR = build/

BOOT_BIN    = $(BUILD_DIR)boot.bin
LOADER_BIN  = $(BUILD_DIR)loader.bin
KERNEL_FILE = $(BUILD_DIR)kernel.bin
OS_IMG      = os.img

MODE = __DEBUG__

ASM_KERNEL_FLAGS = -f elf32
C_KERNEL_FLAGS = -I ./include -m32 -D$(MODE) -c -Wall -nostdlib -fno-builtin -fno-leading-underscore
LD_FLAGS = -m elf_i386 -e _START -Ttext 0x80100000

OBJS = \
	$(BUILD_DIR)_Start.o\
	$(BUILD_DIR)_IO.o\
	$(BUILD_DIR)_Interrupt.o\
	$(BUILD_DIR)start.o\
	$(BUILD_DIR)vbe.o\
	$(BUILD_DIR)io.o\
	$(BUILD_DIR)interrupt.o\
	$(BUILD_DIR)timer.o\
	$(BUILD_DIR)rtc.o\
	$(BUILD_DIR)string.o\
	$(BUILD_DIR)console.o\
	$(BUILD_DIR)text.o\
	$(BUILD_DIR)printk.o

.PHONY: OS_LINK os.img

OS_LINK: $(BOOT_BIN) $(LOADER_BIN) $(KERNEL_FILE)

os.img:
	$(DD) if=$(BOOT_BIN) of=$(OS_IMG) bs=512 count=1 conv=notrunc
	$(DD) if=$(LOADER_BIN) of=$(OS_IMG) bs=512 seek=$(LOAD_SECTOR_OFFSET) count=$(LOAD_SECTORS) conv=notrunc
	$(DD) if=$(KERNEL_FILE) of=$(OS_IMG) bs=512 seek=$(KERNEL_SECTOR_OFFEST) count=$(KERNEL_SECTORS) conv=notrunc

run: .PHONY
	$(QEMU) -boot a -fda $(OS_IMG)

clean:
	rm $(BUILD_DIR)*.bin
	rm $(BUILD_DIR)*.o
	rm *.img
	qemu-img create $(OS_IMG) 1440K
#echo "times 1474560 db 0" > floppy.asm
#$(NASM) floppy.asm -o $(OS_IMG)
#rm floppy.asm

Compile:
#BootLoader
$(BOOT_BIN) : boot/boot.asm
	$(NASM) -o $@ $<
$(LOADER_BIN) : boot/loader.asm
	$(NASM) -o $@ $<

#LINK
$(KERNEL_FILE) : $(OBJS)
	$(LD) $(LD_FLAGS) -o $(KERNEL_FILE) $(OBJS)

#ASM
$(BUILD_DIR)%.o : kernel/%.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<

#C/CPP
$(BUILD_DIR)%.o : kernel/%.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
$(BUILD_DIR)%.o : libraries/%.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<