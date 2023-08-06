#ifndef CS_GEOMETRY_VECTOR3_
#define CS_GEOMETRY_VECTOR3_

#include <cmath>

#include "cs/geometry/point3.h"

namespace cs::geometry {

class Vector3 {
 public:
  Vector3(Point3 b) : a(Point3(0, 0, 0)), b(b) {}
  Vector3(Point3 a, Point3 b) : a(a), b(b) {}
  Point3 a, b;
  float magnitude() const {
    float x_diff = b.x - a.x;
    float y_diff = b.y - a.y;
    float z_diff = b.z - a.z;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff +
                     z_diff * z_diff);
  }
  Vector3 operator+(const Vector3& other) const {
    return Vector3(a + other.a, b + other.b);
  }
  bool operator==(const Vector3& other) const {
    return a == other.a && b == other.b;
  }
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_VECTOR3_