.PHONY: all bootloader kernel clean

BOOTLOADER_DIR:=bootloader
KERNEL_DIR:=kernel

include bootloader/make.config
include kernel/make.config

all: bootloader.img qemu

bootloader:
	cd bootloader && $(MAKE)

kernel:
	cd kernel && $(MAKE)

bootloader.bin: bootloader kernel
	i686-elf-ld -T link.ld -melf_i386 -o bootloader.elf $(BOOTLOADER_S_OBJS) $(KERNEL_C_OBJS) $(KERNEL_S_OBJS)
	i686-elf-objcopy -O binary bootloader.elf bootloader.bin

bootloader.img: bootloader.bin
	dd if=bootloader.bin of=bootloader.img

clean:
	rm bootloader.elf bootloader.bin bootloader.img
	cd bootloader && $(MAKE) clean
	cd kernel && $(MAKE) clean
