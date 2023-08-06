#ifndef CS_GEOMETRY_POINT3_
#define CS_GEOMETRY_POINT3_

#include <iostream>

namespace cs::geometry {

class Point3 {
 public:
  Point3(float x, float y, float z) : x(x), y(y), z(z) {}
  Point3 operator+(const Point3& other) const {
    return Point3(x + other.x, y + other.y, z + other.z);
  }
  bool operator==(const Point3& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
  friend std::ostream& operator<<(std::ostream& os,
                                  const Point3& point) {
    return os << "Point3(" << point.x << ", " << point.y
              << ", " << point.z << ")";
  }
  float x, y, z;
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_POINT3_