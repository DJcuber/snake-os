#include <stddef.h>
#include <stdint.h>

#include "snake.h"
#include "video.h"
#include "utils.h"
#include "timer.h"
#include "keyboard.h"

#define BORDER_COLOR         0x15
#define SNAKE_COLOR          0x2F
#define APPLE_COLOR          0x28

#define GRID_WIDTH           20
#define GRID_HEIGHT          20

#define SQUARE_WIDTH         5
#define BORDER_WIDTH         5
#define GRID_WIDTH_PIXELS   (SQUARE_WIDTH * GRID_WIDTH  + (GRID_WIDTH-1))
#define GRID_HEIGHT_PIXELS  (SQUARE_WIDTH * GRID_HEIGHT + (GRID_HEIGHT-1)) 
#define GRID_OFFSET_X       ((VGA_WIDTH - (GRID_WIDTH_PIXELS + 2*BORDER_WIDTH)) / 2)
#define GRID_OFFSET_Y       ((VGA_HEIGHT - (GRID_HEIGHT_PIXELS + 2*BORDER_WIDTH)) / 2)

#define RIGHT                0
#define UP                   1
#define LEFT                 2
#define DOWN                 3

#define SPEED_SLOW           32
#define SPEED_MEDIUM         16
#define SPEED_FAST           8

#define GAME_SPEED           SPEED_MEDIUM

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
  draw_rect((SQUARE_WIDTH+1)*x + GRID_OFFSET_X + BORDER_WIDTH, (SQUARE_WIDTH+1)*y + GRID_OFFSET_Y + BORDER_WIDTH, SQUARE_WIDTH, SQUARE_WIDTH, color);
}

static void draw_border() {
  draw_rect(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_WIDTH_PIXELS+2*BORDER_WIDTH, BORDER_WIDTH, BORDER_COLOR);
  draw_rect(GRID_OFFSET_X, GRID_OFFSET_Y+BORDER_WIDTH, BORDER_WIDTH, GRID_HEIGHT_PIXELS+BORDER_WIDTH, BORDER_COLOR);
  draw_rect(GRID_OFFSET_X+BORDER_WIDTH+GRID_WIDTH_PIXELS, GRID_OFFSET_Y+BORDER_WIDTH, BORDER_WIDTH, GRID_HEIGHT_PIXELS+BORDER_WIDTH, BORDER_COLOR);
  draw_rect(GRID_OFFSET_X+BORDER_WIDTH, GRID_OFFSET_Y+BORDER_WIDTH+GRID_HEIGHT_PIXELS, GRID_WIDTH_PIXELS, BORDER_WIDTH, BORDER_COLOR);
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
  data |= apple << 2;
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
    case RIGHT:
      (*x)++;
      break;
    case UP:
      (*y)--;
      break;
    case LEFT:
      (*x)--;
      break;
    case DOWN:
      (*y)++;
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

  if (bounds_check == false || (((next_square >> 3) & 1) == 1 && ((next_square >> 2) & 1) == 0)) {
    game_over = true;
    return;
  }

  place_obj(new_x, new_y, false, 0);
  remove_obj(snake.head_x, snake.head_y);
  place_obj(snake.head_x, snake.head_y, false, snake.direction);

  snake.head_x = new_x;
  snake.head_y = new_y;

  if ((next_square & 4) != 0)  {
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
    uint8_t start = tick / GAME_SPEED;
    // do game loop
    move_snake();

    uint16_t input = kb_buffer_pop();

    switch (input) {
      case KB_D:
      case KB_RIGHT:
        if (snake.direction != LEFT)
          snake.direction = RIGHT;
        break;

      case KB_W:
      case KB_UP:
        if (snake.direction != DOWN)
          snake.direction = UP;
        break;

      case KB_A:
      case KB_LEFT:
        if (snake.direction != RIGHT)
          snake.direction = LEFT;
        break;

      case KB_S:
      case KB_DOWN:
        if (snake.direction != UP)
          snake.direction = DOWN;
        break; 
    }
    
    while (start == tick / GAME_SPEED) {
      asm volatile ("hlt");
    }
  }
  // maybe play some animation when game is over
}

void init_game() {
  draw_border();

  snake.direction = 0; 
  snake.head_x = GRID_WIDTH/2 + 2;
  snake.head_y = GRID_HEIGHT/2;
  snake.tail_x = GRID_WIDTH/2 - 1;
  snake.tail_y = GRID_HEIGHT/2;
  snake.size = 4;

  place_obj(GRID_WIDTH/2 - 1, GRID_HEIGHT/2, 0, 0);
  place_obj(GRID_WIDTH/2, GRID_HEIGHT/2, 0, 0);
  place_obj(GRID_WIDTH/2 + 1, GRID_HEIGHT/2, 0, 0);
  place_obj(GRID_WIDTH/2 + 2, GRID_HEIGHT/2, 0, 0);

  place_apple();

  game_loop();
}

