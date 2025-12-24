#ifndef _VIDEO_H

#include <stddef.h>
#include <stdint.h>

// #include "font.h"

#define _VIDEO_H

#define VGA_WIDTH 320
#define VGA_HEIGHT 200

void border();

void put_pixel(unsigned char color, size_t x, size_t y);

// void drawGlyph(Font font, size_t x, size_t y);

#endif

