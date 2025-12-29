#include <stddef.h>
#include <stdint.h>

#include "snake.h"
#include "video.h"
#include "utils.h"
#include "timer.h"

#define BORDER_COLOR 0x15
#define SNAKE_COLOR  0x2F
#define APPLE_COLOR  0x28

#define GRID_WIDTH   50
#define GRID_HEIGHT  30

/* 
 * Each square contains 4 bits, 2 squares in a grid entry:
 * Bit 4: 1 if square occupied (with an apple or a snake segment)
 * Bit 3: 1 if square is an apple
 * Bit 1 & 2: direction of previous segment
 *  00: right
 *  01: up
 *  10: left
 *  11: down
 */
uint8_t grid[GRID_HEIGHT][GRID_WIDTH/2];

extern uint8_t tick;
extern void wait(uint8_t);

snake_t snake;
bool game_over = false;


/*
 * grid: 50 x 30
 * square: 5 x 5 (1 px border)
 * width: 5 * 50 + 48 + 10 = 308
 * height: 5 * 30 + 28 + 10 = 188
 */

static void draw_square(size_t x, size_t y, unsigned char color) {
  draw_rect(6 * x + 11, 6 * y + 11, 5, 5, color);
}

static void draw_border() {
  draw_rect(6, 6, VGA_WIDTH-11, 5, BORDER_COLOR);
  draw_rect(6, 11, 5, VGA_HEIGHT-16, BORDER_COLOR);
  draw_rect(VGA_WIDTH-10, 11, 5, VGA_HEIGHT-16, BORDER_COLOR);
  draw_rect(6, VGA_HEIGHT-10, VGA_WIDTH-11, 5, BORDER_COLOR);

  /*
  vga_buffer[0] = 100;
  vga_buffer[VGA_WIDTH-1] = 100;
  vga_buffer[VGA_WIDTH*(VGA_HEIGHT-1)] = 100;
  vga_buffer[VGA_WIDTH*VGA_HEIGHT-1] = 100;
  */
}

static inline uint8_t get_square(size_t x, size_t y) {
  if ((x % 2) == 0) {
    return grid[y][x/2] & 0x0F;
  }
  return (grid[y][x/2] & 0xF0) >> 4;
}

static void place_obj(size_t x, size_t y, bool apple, size_t direction) {
  grid[y][x/2] &= 0x00 | (0x0F << (4 * (1 - x % 2)));

  uint8_t data = 8;
  data |= apple << 3;
  data |= direction;

  grid[y][x/2] |= data << (4 * (x % 2));
  draw_square(x, y, (apple ? APPLE_COLOR : SNAKE_COLOR));
}

static inline void remove_obj(size_t x, size_t y) {
  if ((x % 2) == 0) {
    grid[y][x/2] &= 0xF0;
  }
  else {
    grid[y][x/2] &= 0x0F;
  }
  draw_square(x, y, 0);
}

static void place_apple() {
  while (snake.size != GRID_WIDTH * GRID_HEIGHT) {
    uint32_t rand = cpu_rand() % (GRID_WIDTH * GRID_HEIGHT);
    if ((get_square(rand % GRID_WIDTH, rand/GRID_WIDTH) & 8) == 0) {
      place_obj(rand%GRID_WIDTH, rand/GRID_WIDTH, true, 0);
      break;
    }
  }
}

static void get_offset(uint8_t* x, uint8_t* y, uint8_t direction) {
  switch (direction) {
    case 0:
      (*x)++;
      break;
    case 1:
      (*y)++;
      break;
    case 2:
      (*x)--;
      break;
    case 3:
      (*y)--;
      break;
  }
}

static void move_snake() {
  uint8_t new_x = snake.head_x;
  uint8_t new_y = snake.head_y;

  get_offset(&new_x, &new_y, snake.direction);

  bool bounds_check = new_x < GRID_WIDTH;
  bounds_check &= new_y < GRID_HEIGHT;

  uint8_t next_square = get_square(new_x, new_y);

  if (bounds_check == false || (next_square & 8 == 1 && next_square & 4 == 0)) {
    game_over = true;
    return;
  }

  place_obj(new_x, new_y, false, 0);
  remove_obj(snake.head_x, snake.head_y);
  place_obj(snake.head_x, snake.head_y, false, snake.direction);

  snake.head_x = new_x;
  snake.head_y = new_y;

  if (next_square & 8 == 1)  {
    // eat apple
    snake.size++;
    place_apple();
  }
  else {
    // move tail up
    new_x = snake.tail_x;
    new_y = snake.tail_y;
 
    uint8_t direction = get_square(snake.tail_x, snake.tail_y) & 0x03;
    get_offset(&new_x, &new_y, direction);
    remove_obj(snake.tail_x, snake.tail_y);

    snake.tail_x = new_x;
    snake.tail_y = new_y; 
  }
}

void game_loop() {
  while (!game_over) {
    uint8_t start = tick / 16;
    // do game loop
    move_snake();
    
    while (start == tick/16) {
      asm volatile ("hlt");
    }
  }
  // maybe play some animation when game is over
}

void init_game() {
  draw_border();

  snake.direction = 0; 
  snake.head_x = 26;
  snake.head_y = 14;
  snake.tail_x = 23;
  snake.tail_y = 14;
  snake.size = 4;

  place_obj(23, 14, 0, 0);
  place_obj(24, 14, 0, 0);
  place_obj(25, 14, 0, 0);
  place_obj(26, 14, 0, 0);

  place_apple();

  game_loop();
}

