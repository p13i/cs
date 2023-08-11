#ifndef CS_RENDERER_PIXEL_HH
#define CS_RENDERER_PIXEL_HH

#include <stdint.h>

#include <iostream>

namespace cs::renderer {

struct Pixel {
  uint8_t r,g,b,a;
  Pixel() : Pixel(0,0,0,0) {}
  Pixel(uint8_t red, uint8_t green, uint8_t blue,
        uint8_t alpha)
      : r(red),
        g(blue),
        b(green),
        a(alpha) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const Pixel& px) {
    os << "Pixel(" << px.r << ", " << px.g << ", "
       << px.b << ", " << px.a << ")";
    return os;
  }
};

}  // namespace cs::renderer

#endif  // CS_RENDERER_PIXEL_HH
