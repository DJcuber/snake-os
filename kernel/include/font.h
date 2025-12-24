#ifndef _FONT_H
#define _FONT_H

#include <stdint.h>
#include <stddef.h>

struct Glyph {
  uint8_t pixels[16];
};

struct Font {
  struct Glyph map[128];
};

#endif
