#include "video.h"

unsigned char* vga_buffer = (unsigned char*) 0xA0000;
/*
 * grid: 30 x 50
 * square: 5 x 5 (1 px border)
 * width: 5 * 50 + 48 + 10 = 308
 * height: 5 * 30 + 28 + 10 = 188
 */
void border() {
  // gray: 0x15
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < VGA_WIDTH-12; j++) {
      vga_buffer[(i+6)*VGA_WIDTH+j+6] = 0x15;
      vga_buffer[VGA_WIDTH*(VGA_HEIGHT-11+i)+j+6] = 0x15;
    }
    for (size_t j = 11; j < VGA_HEIGHT-11; j++) {
      vga_buffer[j*VGA_WIDTH+i+6] = 0x15;
      vga_buffer[j*VGA_WIDTH+VGA_WIDTH-i-7] = 0x15;
    }
  }

  /*
  vga_buffer[0] = 100;
  vga_buffer[VGA_WIDTH-1] = 100;
  vga_buffer[VGA_WIDTH*(VGA_HEIGHT-1)] = 100;
  vga_buffer[VGA_WIDTH*VGA_HEIGHT-1] = 100;
  */
}

void put_pixel(unsigned char color, size_t x, size_t y) {
  vga_buffer[VGA_WIDTH*y + x] = color;
}

/*
void drawGlyph(unsigned char* buffer, Glyph glyph, size_t x, size_t y) {
}
*/
