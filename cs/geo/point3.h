#ifndef CS_GEO_POINT3_
#define CS_GEO_POINT3_

#include <iostream>

#include "cs/math/constants.h"

namespace cs::geo {

class Point3 {
 public:
  Point3() : x(0), y(0), z(0) {}

  Point3(float x, float y, float z) : x(x), y(y), z(z) {}

  Point3 operator+(const Point3& other) const {
    return Point3(x + other.x, y + other.y, z + other.z);
  }

  bool operator==(const Point3& other) const {
    return FLOAT_NEAR(x, other.x) && FLOAT_NEAR(y, other.y) && FLOAT_NEAR(z, other.z);
  }

  Point3 operator-(Point3 other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  Point3 operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  Point3 operator/(float scalar) const {
    return {x / scalar, y / scalar, z / scalar};
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