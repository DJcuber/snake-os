AS:=nasm

BOOTLOADER_DIR:=.

include make.config

.PHONY: all clean

all: $(BOOTLOADER_S_OBJS)

$(BOOTLOADER_S_OBJS): %.o : %.asm
	$(AS) -f elf32 $< -o $@

clean:
	rm $(BOOTLOADER_S_OBJS) || true
