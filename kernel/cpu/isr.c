#include "isr.h"
#include "idt.h"
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
  if (regs.int_no >= ICW2_OFFSET + 8) {
    // Reset slave signal
    outb(PIC2_COMMAND_PORT, PIC_RESET);
  }
  // Reset master signal
  outb(PIC1_COMMAND_PORT, PIC_RESET);

  if (interrupt_callbacks[regs.int_no] != 0) {
    isr_t handler = interrupt_callbacks[regs.int_no];
    handler(regs);
  }
}
