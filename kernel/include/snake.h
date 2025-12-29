#ifndef _SNAKE_H
#define _SNAKE_H

void init_game();

typedef struct {
  uint8_t direction;
  uint8_t head_x;
  uint8_t head_y;
  uint8_t tail_x;
  uint8_t tail_y;
  uint8_t size;
} snake_t;

#endif
