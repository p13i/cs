#ifndef CS_GEO_VECTOR3_
#define CS_GEO_VECTOR3_

#include <cmath>
#include <iostream>

#include "cs/geo/point3.h"

namespace cs::geo {

class Vector3 {
 public:
  Vector3() : Vector3(Point3()) {}

  Vector3(Point3 b) : Vector3(Point3(), b) {}

  Vector3(Point3 a, Point3 b) : a(a), b(b) {}

  float magnitude() const {
    Point3 diff = b - a;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y +
                     diff.z * diff.z);
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

}  // namespace cs::geo

#endif  // CS_GEO_VECTOR3_