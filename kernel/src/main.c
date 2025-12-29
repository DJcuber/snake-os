#include <stddef.h>
#include <stdint.h>

#include "video.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"
#include "snake.h"

void kernel_main() {
  init_idt();

  init_timer(128);
  init_keyboard();

  init_game();
}
