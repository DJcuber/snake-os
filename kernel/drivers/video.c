#include "video.h"


unsigned char* vga_buffer = (unsigned char*) 0xA0000;

void put_pixel(unsigned char color, size_t x, size_t y) {
  vga_buffer[VGA_WIDTH*y + x] = color;
}

void draw_rect(size_t x, size_t y, size_t width, size_t height, unsigned char color) {
  unsigned char* ptr = vga_buffer;
  ptr += VGA_WIDTH * y + x;

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      ptr[j] = color;
    }
    ptr += VGA_WIDTH;
  }
}


/*
void drawGlyph(unsigned char* buffer, Glyph glyph, size_t x, size_t y) {
}
*/
