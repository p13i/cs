#ifndef CS_RENDERER_RAINBOW_H
#define CS_RENDERER_RAINBOW_H

#include <stdint.h>

#include <iostream>

#include "cs/renderer/film.hh"

namespace cs::renderer {

class RainbowRenderer {
 public:
  RainbowRenderer() {}
  void render(Film* film);
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_RAINBOW_H