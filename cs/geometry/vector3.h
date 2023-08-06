#ifndef CS_GEOMETRY_VECTOR3_
#define CS_GEOMETRY_VECTOR3_

#include "cs/geometry/point3.h"

namespace cs::geometry {

class Vector3 {
 public:
  Vector3(Point3 a, Point3 b) : a(a), b(b) {}
  Point3 a, b;
  Vector3 operator+(const Vector3& other) const {
    return Vector3(a + other.a, b + other.b);
  }
  bool operator==(const Vector3& other) const {
    return a == other.a && b == other.b;
  }
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_VECTOR3_