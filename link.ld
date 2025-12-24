ENTRY(kernel_main)

SECTIONS {
  . = 0x7C00;
  .entry : AT(0x7C00) SUBALIGN(4) {
    bootloader/entry.o(.text)
    bootloader/entry.o(.*)
  }

  . = 0x7DFE;
  .bootsig : {
      SHORT(0xAA55);
  }
  
  . = 0x7E00;
  .stage2 : AT(0x7E00) SUBALIGN(4) {
    bootloader/stage2.o(.text)
  }

  . = 0x8000;
  .kernel : SUBALIGN(4) {
    *(.text*)
    *(.rodata*)
    *(.data*)
  }

  .bss : SUBALIGN(4) {
    __bss_start = .;
    *(COMMON)
    *(.bss*)
  }
  . = ALIGN(4);
  __bss_end = .;

  kernel_sectors = (SIZEOF(.kernel) + 511) / 512;
}
