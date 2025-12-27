#include "idt.h"
#include "utils.h"

static void idt_set(uint8_t gate_no, uint32_t offset);
static void set_gate(uint8_t gate_no, uint32_t offset, uint16_t selector, uint8_t flags);

static void remap_PIC();

idt_gate_t gates[256];
idt_descriptor_t descriptor;

void init_idt() {
  descriptor.limit = sizeof(idt_gate_t) * 256 - 1;
  descriptor.base = (uint32_t)&gates;

  remap_PIC();

  // Sets interrupt service routines
  idt_set(0 , (uint32_t)isr0 );
  idt_set(1 , (uint32_t)isr1 );
  idt_set(2 , (uint32_t)isr2 );
  idt_set(3 , (uint32_t)isr3 );
  idt_set(4 , (uint32_t)isr4 );
  idt_set(5 , (uint32_t)isr5 );
  idt_set(6 , (uint32_t)isr6 );
  idt_set(7 , (uint32_t)isr7 );
  idt_set(8 , (uint32_t)isr8 );
  idt_set(9 , (uint32_t)isr9 );
  idt_set(10, (uint32_t)isr10);
  idt_set(11, (uint32_t)isr11);
  idt_set(12, (uint32_t)isr12);
  idt_set(13, (uint32_t)isr13);
  idt_set(14, (uint32_t)isr14);
  idt_set(15, (uint32_t)isr15);
  idt_set(16, (uint32_t)isr16);
  idt_set(17, (uint32_t)isr17);
  idt_set(18, (uint32_t)isr18);
  idt_set(19, (uint32_t)isr19);
  idt_set(20, (uint32_t)isr20);
  idt_set(21, (uint32_t)isr21);
  idt_set(22, (uint32_t)isr22);
  idt_set(23, (uint32_t)isr23);
  idt_set(24, (uint32_t)isr24);
  idt_set(25, (uint32_t)isr25);
  idt_set(26, (uint32_t)isr26);
  idt_set(27, (uint32_t)isr27);
  idt_set(28, (uint32_t)isr28);
  idt_set(29, (uint32_t)isr29);
  idt_set(30, (uint32_t)isr30);
  idt_set(31, (uint32_t)isr31);

  // Sets interrupt requests
  idt_set(32, (uint32_t)irq0 );
  idt_set(33, (uint32_t)irq1 );
  idt_set(34, (uint32_t)irq2 );
  idt_set(35, (uint32_t)irq3 );
  idt_set(36, (uint32_t)irq4 );
  idt_set(37, (uint32_t)irq5 );
  idt_set(38, (uint32_t)irq6 );
  idt_set(39, (uint32_t)irq7 );
  idt_set(40, (uint32_t)irq8 );
  idt_set(41, (uint32_t)irq9 );
  idt_set(42, (uint32_t)irq10);
  idt_set(43, (uint32_t)irq11);
  idt_set(44, (uint32_t)irq12);
  idt_set(45, (uint32_t)irq13);
  idt_set(46, (uint32_t)irq14);
  idt_set(47, (uint32_t)irq15);

  idt_flush((uint32_t)&descriptor);
}

static void remap_PIC() {
  // ICW1: Tell the PIC's that they are going to be remapped
  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  // ICW2: PIC vector offset
  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  // ICW3: Tell the PIC's how they are related to each other
  outb(0x21, 0x04);
  outb(0xA1, 0x02);

  // ICW4: Use 8086 mode
  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  // Unmask interrupt
  outb(0x21, 0x00);
  outb(0xA1, 0x00);
}

static void idt_set(uint8_t gate_no, uint32_t offset) {
  set_gate(gate_no, offset, 0x08, 0x8E);
}

static void set_gate(uint8_t gate_no, uint32_t offset, uint16_t selector, uint8_t flags) {
  gates[gate_no].offset_low = 0xFFFF & offset;
  gates[gate_no].offset_high = (0xFFFF << 16) & offset;
  
  gates[gate_no].seg_selector = selector;
  gates[gate_no].res = 0;
  gates[gate_no].flags = flags;
}
