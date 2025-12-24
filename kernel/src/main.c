#include <stddef.h>
#include <stdint.h>

#include "video.h"
#include "idt.h"

void kernel_main() {
  init_idt();
  border();

  int c = 5 / 0;
  // asm volatile("int $0x05");

  /*
  int base = 32;
  for (size_t i = 0; i < 22; i++) {
    vga_buffer[i] = base + i;
  }
  */
}
