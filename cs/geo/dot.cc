#include "cs/geo/dot.hh"

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"
#include "cs/math/constants.h"
#include "cs/sanity/ensure.hh"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

namespace cs::geo {
// Performs a standard dot product.
float dot(const p3& a, const p3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot(const v3& a, const v3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
}  // namespace cs::geo
