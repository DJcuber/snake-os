[BITS 16]

section .text

%include "gdt.asm"

global switch_mode

switch_mode:
  ; set video mode (320 x 200, 8-bit colour)
  mov ah, 0
  mov al, 0x13
  int 0x10

  cli
  lgdt [gdt_descriptor]

  ; enable A20 line
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; enter protected mode (32-bit)
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

  ; initialise stack again
  mov ebp, 0x7C00
  mov esp, ebp

  call kernel_main

loop:
  ; if the program ever exits, it will end up here
  hlt 
  jmp loop
