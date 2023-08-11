#ifndef CS_RENDERER_FILM_HH
#define CS_RENDERER_FILM_HH

#include <stdint.h>

#include <iostream>

#include "cs/renderer/pixel.hh"

namespace cs::renderer {

struct Film {
  uint32_t width, height;
  Pixel** pixels;
  Film(uint32_t width, uint32_t height)
      : width(width), height(height) {
    pixels = new Pixel*[width];
    for (uint32_t w = 0; w < width; w++) {
      pixels[w] = new Pixel[height];
    }
  }
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_FILM_HH