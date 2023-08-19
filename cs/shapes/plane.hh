#ifndef CS_SHAPES_PLANE_HH
#define CS_SHAPES_PLANE_HH

#include <cmath>

#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/shapes/shape.hh"

using r3 = ::cs::geo::Ray3;
using v3 = ::cs::geo::Vector3;
using p3 = ::cs::geo::Point3;

namespace cs::shapes {
struct Plane : public Shape {
  float a, b, c, d;

  Plane(p3 abc, float d) : Plane(abc.x, abc.y, abc.z, d) {}

  Plane(float a, float b, float c, float d)
      : a(a), b(b), c(c), d(d) {}

  bool intersected_by(r3 ray, p3* atPoint,
                      v3* atNormal) override;
};
}  // namespace cs::shapes

#endif  // CS_SHAPES_PLANE_HH