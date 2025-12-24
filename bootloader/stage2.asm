[BITS 16]

section .text

%include "gdt.asm"

global switch_mode

switch_mode:
  ; set video mode
  mov ah, 0
  mov al, 0x13
  int 0x10

  cli
  lgdt [gdt_descriptor]

  ; enable A20 line
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; far jump sets cs implicitly
  jmp 0x08:p_mode


[BITS 32]
extern kernel_main

p_mode:
  mov ax, 0x10
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000
  mov esp, ebp

  call kernel_main
  hlt 
