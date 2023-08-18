#ifndef CS_SHAPES_SHAPE_HH
#define CS_SHAPES_SHAPE_HH

#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

namespace cs::shapes {
class Shape {
 public:
  virtual bool intersected_by(r3 ray, p3* atPoint,
                              v3* atNormal) = 0;
  virtual ~Shape() = default;
};
};  // namespace cs::shapes

#endif  // CS_SHAPES_SHAPE_HH