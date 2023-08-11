#ifndef CS_RENDERER_RAINBOW_H
#define CS_RENDERER_RAINBOW_H

#include <stdint.h>

#include <iostream>

namespace cs::renderer {

struct Pixel {
  uint8_t red_, blue_, green_, alpha_;
  Pixel() : red_(0), blue_(0), green_(0), alpha_(0) {}
  Pixel(uint8_t red, uint8_t blue, uint8_t green,
        uint8_t alpha)
      : red_(red),
        blue_(blue),
        green_(green),
        alpha_(alpha) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const Pixel& px) {
    os << "Pixel(" << px.red_ << ", " << px.blue_ << ", "
       << px.green_ << ", " << px.alpha_ << ")";
    return os;
  }
};

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

class RainbowRenderer {
 public:
  RainbowRenderer() {}
  void render(Film* film);
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_RAINBOW_H