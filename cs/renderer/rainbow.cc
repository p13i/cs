#include "cs/renderer/rainbow.h"

void cs::renderer::RainbowRenderer::render(Film* film) {
  for (uint32_t i = 0; i < film->width; i++) {
    for (uint32_t j = 0; j < film->height; j++) {
      float i_ratio = i / (float)film->width,
            j_ratio = j / (float)film->height;
      film->pixels[i][j] =
          cs::renderer::Pixel(i_ratio * 255, j_ratio * 255, (1 - i_ratio) * 255, 255);
    }
  }
};
