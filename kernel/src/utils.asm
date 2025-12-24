global outb
outb:
  mov al, [esp+4]
  mov dx, [esp+8]
  out dx, al
  ret

global inb
inb:  
  mov dx, [esp+4]
  in al, dx
  ret
