#ifndef CS_RENDERER_FILM_HH
#define CS_RENDERER_FILM_HH

#include <stdint.h>

#include <iostream>

#include "cs/collections/tuple.hh"
#include "cs/renderer/pixel.hh"

using ::cs::collections::Tuple;

namespace cs::renderer {

struct Film {
  unsigned int width, height;
  Pixel** pixels;
  Film() : Film(0, 0) {}
  Film(Tuple<unsigned int, unsigned int> dimensions)
      : Film(dimensions.first(), dimensions.second()) {}
  Film(unsigned int width, unsigned int height)
      : width(width), height(height) {
    pixels = new Pixel*[width];
    for (unsigned int w = 0; w < width; w++) {
      pixels[w] = new Pixel[height];
    }
  }
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_FILM_HH