.PHONY: all bootloader kernel clean

BOOTLOADER_DIR:=bootloader
KERNEL_DIR:=kernel

include bootloader/make.config
include kernel/make.config

all: os.img

bootloader:
	cd bootloader && $(MAKE)

kernel:
	cd kernel && $(MAKE)

os.bin: bootloader kernel
	i686-elf-ld -T link.ld -melf_i386 -o os.elf $(BOOTLOADER_S_OBJS) $(KERNEL_C_OBJS) $(KERNEL_S_OBJS)
	i686-elf-objcopy -O binary os.elf os.bin

os.img: os.bin
	dd if=os.bin of=os.img

clean:
	rm os.elf os.bin os.img || true
	cd bootloader && $(MAKE) clean
	cd kernel && $(MAKE) clean
