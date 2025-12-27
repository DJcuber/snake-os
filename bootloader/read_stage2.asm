[BITS 16]

read:
  xor ax, ax 

  int 0x13

  mov es, ax
  mov bx, 0x7E00

  mov ah, 0x02
  mov al, 31 ;TODO
  mov ch, 0
  mov cl, 2

  mov dh, 0

  int 0x13
  ret
