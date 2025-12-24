#include "isr.h"
#include "video.h"
#include "utils.h"

void isr_handler(registers_t regs) {
  put_pixel(0x20+regs.int_no, 0, 0);
}

void irq_handler(registers_t regs) {
  if (regs.int_no >= 0x28) {
    // Reset slave signal
    outb(0xA0, 0x20);
  }
  // Reset master signal
  outb(0x20, 0x20);

  put_pixel(regs.int_no, 0, VGA_WIDTH-1);
  //TODO: make actual handler
}
