#ifndef _VIDEO_H
#define _VIDEO_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 320
#define VGA_HEIGHT 200

void border();

// WARNING: Bounds for these functions are not checked
void put_pixel(unsigned char color, size_t x, size_t y);

void draw_rect(size_t x, size_t y, size_t width, size_t height, unsigned char color);

#endif

