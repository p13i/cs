#ifndef CS_GEO_POINT3_
#define CS_GEO_POINT3_

#include <iostream>
#include <cmath>

#include "cs/precision/floats.hh"

using ::cs::precision::FloatsNear;

namespace cs::geo {

class Point3 {
 public:
  Point3() : Point3(0, 0, 0) {}

  Point3(float xyz) : Point3(xyz, xyz, xyz) {}

  Point3(float x, float y, float z) : x(x), y(y), z(z) {}

  Point3 unit() const {
    Point3 this_ = *this;
    return this_ /
           sqrtf(this_.x * this_.x + this_.y * this_.y +
                 this_.z * this_.z);
  }

  Point3 operator+(const Point3& other) const {
    return Point3(x + other.x, y + other.y, z + other.z);
  }

  bool operator==(const Point3& other) const {
    return FloatsNear(x, other.x) &&
           FloatsNear(y, other.y) && FloatsNear(z, other.z);
  }

  Point3 operator-(Point3 other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  Point3 operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  Point3 operator*(Point3 other) const {
    return {x * other.x, y * other.y, z * other.z};
  }

  Point3 operator/(float scalar) const {
    return {x / scalar, y / scalar, z / scalar};
  }

  Point3 operator/(Point3 other) const {
    return {x / other.x, y / other.y, z / other.z};
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Point3& point) {
    return os << "Point3(" << point.x << ", " << point.y
              << ", " << point.z << ")";
  }

  float x, y, z;
};

}  // namespace cs::geo

#endif  // CS_GEO_POINT3_