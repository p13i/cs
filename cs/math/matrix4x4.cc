#include "cs/math/matrix4x4.hh"

#include "cs/math/in_range.hh"
#include "cs/sanity/ensure.hh"

using ::cs::math::in_range;
using ::cs::math::Matrix4x4;

#include <memory.h>
#include <stdint.h>

namespace {

void EnsureIndexArgs(uint8_t x, uint8_t y) {
  ENSURE(in_range<uint8_t>(x, 0, 4));
  ENSURE(in_range<uint8_t>(y, 0, 4));
}

}  // namespace

float cs::math::Matrix4x4::get(uint8_t x, uint8_t y) const {
  EnsureIndexArgs(x, y);
  return data_[x][y];
}

void cs::math::Matrix4x4::set(uint8_t x, uint8_t y,
                              float datum) {
  EnsureIndexArgs(x, y);
  data_[x][y] = datum;
}
