global outb
outb:
  mov al, [esp+8]
  mov dx, [esp+4]
  out dx, al
  ret

global inb
inb:  
  mov dx, [esp+4]
  in al, dx
  ret

global cpu_rand
cpu_rand:
  rdrand eax
  ret
