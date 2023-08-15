#include "cs/math/polar2cart.hh"

Point3 cs::math::polar2cart(float r, float theta,
                            float phi) {
  return {/*x=*/r * sinf(theta) * cosf(phi),
          /*y=*/r * sinf(theta) * sinf(phi),
          /*z=*/r * cosf(theta)};
};
