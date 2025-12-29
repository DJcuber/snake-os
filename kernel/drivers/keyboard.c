#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"
#include "isr.h"
#include "utils.h"
#include "video.h"

static bool extended = false;

static void keyboard_callback(registers_t regs) {
  uint16_t sc = inb(0x60);

  if (sc == 0xE0) {
    extended = true;
    return;
  }
  if (extended) {
    sc |= (0xE0 << 8);
    extended = false;
  }

  // TODO: 
  if (sc == KB_SPACE)
    put_pixel(0x20, VGA_WIDTH-1, 0);
  else if (sc == KB_RIGHT)
    put_pixel(0x28, VGA_WIDTH-1, 0);
  else
    put_pixel(0x00, VGA_WIDTH-1, 0);
}

void init_keyboard() {
  set_isr_callback(33, &keyboard_callback);
}
