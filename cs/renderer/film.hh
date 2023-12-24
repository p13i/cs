#ifndef CS_RENDERER_FILM_HH
#define CS_RENDERER_FILM_HH

#include <stdint.h>

#include <iostream>
#include <tuple>

#include "cs/renderer/pixel.hh"

namespace cs::renderer {

struct Film {
  unsigned int width, height;
  Pixel** pixels;
  Film() : Film(0, 0) {}
  Film(std::tuple<unsigned int, unsigned int> dimensions)
      : Film(std::get<0>(dimensions),
             std::get<1>(dimensions)) {}
  Film(unsigned int width, unsigned int height)
      : width(width), height(height) {
    pixels = new Pixel*[width];
    for (unsigned int w = 0; w < width; w++) {
      pixels[w] = new Pixel[height];
    }
  }

  std::tuple<unsigned int, unsigned int> dimensions()
      const {
    return {width, height};
  }
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_FILM_HH