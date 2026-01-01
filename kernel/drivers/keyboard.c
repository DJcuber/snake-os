#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"
#include "isr.h"
#include "utils.h"
#include "video.h"

static bool extended = false;

queue_t input_buffer;

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

  if (sc == KB_W || sc == KB_A || sc == KB_S || sc == KB_D ||
      sc == KB_UP || sc == KB_LEFT || sc == KB_RIGHT || sc == KB_DOWN) {
    kb_buffer_push(sc);
  }
}

static void kb_buffer_init() {
  input_buffer.front = 0;
  input_buffer.back = 9;
}

void kb_buffer_push(uint16_t sc) {
  size_t next = (input_buffer.back+1) % BUFFER_SIZE;
  if (input_buffer.front == next && input_buffer.buffer[next] != 0) {
    return;
  }
  
  input_buffer.buffer[next] = sc;
  input_buffer.back = next;
}

uint16_t kb_buffer_pop() {
  if (input_buffer.buffer[input_buffer.front] == 0) {
    return 0;
  }
  uint16_t curr = input_buffer.buffer[input_buffer.front];
  input_buffer.buffer[input_buffer.front] = 0;
  input_buffer.front = (input_buffer.front+1) % BUFFER_SIZE;
  return curr;
}

void init_keyboard() {
  set_isr_callback(33, &keyboard_callback);
  kb_buffer_init();
}
