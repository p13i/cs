#ifndef CS_SHAPES_SPHERE_HH
#define CS_SHAPES_SPHERE_HH

#include <cmath>

#include "cs/geo/dot.hh"
#include "cs/geo/ray3.h"
#include "cs/math/constants.h"
#include "cs/shapes/shape.hh"
#include "cs/string/format.h"

using r3 = ::cs::geo::Ray3;
using v3 = ::cs::geo::Vector3;
using p3 = ::cs::geo::Point3;
using ::cs::geo::dot;

namespace cs::shapes {
struct Sphere : public Shape {
  p3 center;
  float radius;

  Sphere(p3 center, float radius)
      : center(center), radius(radius) {}

  bool intersected_by(r3 ray, p3* atPoint,
                      v3* atNormal) override;
};
}  // namespace cs::shapes

#endif  // CS_SHAPES_SPHERE_HH