#ifndef CS_GEO_VECTOR3_
#define CS_GEO_VECTOR3_

#include <cmath>
#include <iostream>

#include "cs/geo/point3.h"
#include "cs/precision/floats.hh"

using ::cs::precision::FloatsNear;
using p3 = ::cs::geo::Point3;

namespace cs::geo {

class Vector3 {
 public:
  Vector3() : Vector3(Point3()) {}

  Vector3(p3 point) : Vector3(point.x, point.y, point.z) {}

  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  float magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  bool is_unit() const {
    return FloatsNear(magnitude(), 1.f);
  }

  Vector3 normalized() const { return *this / magnitude(); }

  Vector3 unit() const { return *this / magnitude(); }

  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(Vector3 other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator+(Point3 other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(Point3 other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }

  bool operator==(const Vector3& other) const {
    return FloatsNear(x, other.x) &&
           FloatsNear(y, other.y) && FloatsNear(z, other.z);
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector3& vec) {
    os << "Vector3(" << vec.x << ", " << vec.y << ", "
       << vec.z << ")";
    return os;
  }

  float x, y, z;
};

}  // namespace cs::geo

#endif  // CS_GEO_VECTOR3_