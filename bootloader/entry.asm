[BITS 16]

section .text

extern switch_mode

global _start
_start:
  xor ax, ax

  ; linker script already placed the bootloader at [org 0x7C00]
  mov ds, ax
  mov es, ax
  mov ss, ax

  mov sp, 0x7C00

  call read

  jmp 0:switch_mode

%include "read_stage2.asm"
