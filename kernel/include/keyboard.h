#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stddef.h>

#define BUFFER_SIZE  10


void init_keyboard();

void kb_buffer_push(uint16_t sc);

uint16_t kb_buffer_pop();

enum kb_scancodes {
  KB_W = 0x11,
  KB_A = 0x1E,
  KB_S = 0x1F,
  KB_D = 0x20,

  KB_SPACE = 0x39,

  KB_UP = 0xE048,
  KB_LEFT = 0xE04B,
  KB_RIGHT = 0xE04D,
  KB_DOWN = 0xE050,
};

typedef struct {
  uint16_t buffer[BUFFER_SIZE];
  size_t front;
  size_t back;
} queue_t;

#endif
