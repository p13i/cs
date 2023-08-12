#include "cs/renderer/rainbow.h"

#include <cmath>

#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"

void cs::renderer::RainbowRenderer::render(Film* film) {
  for (uint32_t i = 0; i < film->width; i++) {
    float i_ratio = (i + 0.5) / (float)(film->width);
    for (uint32_t j = 0; j < film->height; j++) {
      float j_ratio = (j + 0.5) / (float)(film->height);
      Pixel pixel = cs::renderer::Pixel(
          std::roundf(i_ratio * 255),
          std::roundf(j_ratio * 255),
          std::roundf((1 - i_ratio) * 255), 255);
      film->pixels[i][j] = pixel;
      ;
    }
  }
};
