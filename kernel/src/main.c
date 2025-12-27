#include <stddef.h>
#include <stdint.h>

#include "video.h"
#include "idt.h"
#include "timer.h"

void kernel_main() {
  init_idt();
  border();

  init_timer(128);

  // asm volatile ("int $0x20");

  // int c = 5 / 0;
  // asm volatile("int $0x05");

  /*
  int base = 32;
  for (size_t i = 0; i < 22; i++) {
    vga_buffer[i] = base + i;
  }
  */
}
