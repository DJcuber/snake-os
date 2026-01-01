[BITS 16]

read:
  xor ax, ax 

  ; reset disk system
  int 0x13

  mov es, ax
  mov bx, 0x7E00

  mov ah, 0x02
  mov al, 31 
  mov ch, 0
  mov cl, 2

  mov dh, 0

  ; load 31 more sectors into memory
  int 0x13
  ret
