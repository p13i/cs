#ifndef CS_GEOMETRY_VECTOR3_
#define CS_GEOMETRY_VECTOR3_

#include <cmath>
#include <iostream>

#include "cs/geometry/point3.h"

namespace cs::geometry {

class Vector3 {
 public:
  Vector3() : Vector3(Point3()) {}

  Vector3(Point3 b) : Vector3(Point3(), b) {}

  Vector3(Point3 a, Point3 b) : a(a), b(b) {}

  float magnitude() const {
    float x_diff = b.x - a.x;
    float y_diff = b.y - a.y;
    float z_diff = b.z - a.z;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff +
                     z_diff * z_diff);
  }

  Vector3 direction() const {
    return Vector3(Point3(), b - a);
  }

  Vector3 unit() const {
    return Vector3(Point3(), direction().b / magnitude());
  }

  Vector3 operator+(const Vector3& other) const {
    return Vector3(a + other.a, b + other.b);
  }

  Vector3 operator-(Vector3 other) const {
    return {a - other.a, b - other.b};
  }

  bool operator==(const Vector3& other) const {
    return a == other.a && b == other.b;
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector3& vec) {
    os << "Vector3(" << vec.a << ", " << vec.b << ")";
    return os;
  }

  Point3 a, b;
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_VECTOR3_