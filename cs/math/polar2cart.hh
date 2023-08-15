#ifndef CS_MATH_POLAR2CART_H
#define CS_MATH_POLAR2CART_H

#include <cmath>

#include "cs/geo/point3.h"

using ::cs::geo::Point3;

namespace cs::math {
Point3 polar2cart(float r, float theta, float phi);
}  // namespace cs::math

#endif  // CS_MATH_POLAR2CART_H