#include "isr.h"
#include "video.h"
#include "utils.h"

isr_t interrupt_callbacks[256];


void set_isr_callback(uint8_t n, isr_t handler) {
  interrupt_callbacks[n] = handler;
}

void isr_handler(registers_t regs) {
  put_pixel(0x20+regs.int_no, 0, 0);
}

void irq_handler(registers_t regs) {
  if (regs.int_no >= 40) {
    // Reset slave signal
    outb(0xA0, 0x20);
  }
  // Reset master signal
  outb(0x20, 0x20);

  put_pixel(regs.int_no, VGA_WIDTH-1, 0); // Remove later

  if (interrupt_callbacks[regs.int_no] != 0) {
    isr_t handler = interrupt_callbacks[regs.int_no];
    handler(regs);
  }
}
