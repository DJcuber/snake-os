KERNEL_DIR=.
GCC:=i686-elf-gcc

AS:=nasm

FLAGS:= -g -m32 -ffreestanding --no-builtin -I include

include make.config

.PHONY: all clean

all: $(KERNEL_C_OBJS) $(KERNEL_S_OBJS)

$(KERNEL_C_OBJS): %.o : %.c
	$(GCC) $(FLAGS) -c $< -o $@

$(KERNEL_S_OBJS): %.o : %.asm
	$(AS) -f elf32 $< -o $@

clean:
	rm $(KERNEL_C_OBJS) $(KERNEL_S_OBJS) || true
