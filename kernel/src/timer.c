#include <stdint.h>

#include "timer.h"
#include "utils.h"
#include "isr.h"
#include "video.h"

static uint8_t tick = 0;

static void timer_callback(registers_t regs) {
  tick++;
  if ((tick % 64) == 0) {
    if (tick == 128)
      tick = 0;
    put_pixel(0x20, 2 + (tick >> 6), 0);
    put_pixel(0x00, 2 + 1 - (tick >> 6), 0);
  }
}

void init_timer(uint32_t freq) {
  set_isr_callback(0x20, &timer_callback);

  outb(0x43, 0x36); // Sets access mode for channel 0 (and sets it to a square wave generator??)
  
  uint32_t divisor = 1193180 / freq;

  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

  outb(0x40, l);
  outb(0x40, h);
}
