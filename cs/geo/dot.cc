#include "cs/geo/dot.hh"

namespace cs::geo {
// Performs a standard dot product.
float dot(const Point3& a, const Point3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
}  // namespace cs::geo
