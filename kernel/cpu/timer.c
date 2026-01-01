#include <stdint.h>

#include "timer.h"
#include "utils.h"
#include "isr.h"
#include "video.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
  tick++;
}

void init_timer(uint32_t freq) {
  set_isr_callback(0x20, &timer_callback);

  outb(0x43, 0x36); // Sets access mode for channel 0 (and sets it to a square wave generator??)
  
  uint32_t divisor = 1193180 / freq;

  uint8_t low = (uint8_t)(divisor & 0xFF);
  uint8_t high = (uint8_t)((divisor>>8) & 0xFF);

  outb(0x40, low);
  outb(0x40, high);
}
