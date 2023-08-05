#ifndef CS_GEOMETRY_POINT3_
#define CS_GEOMETRY_POINT3_

namespace cs::geometry {

class Point3 {
 public:
  Point3(float x, float y, float z) : x(x), y(y), z(z) {}
  float x, y, z;
  Point3 operator+(const Point3& other) const {
    return Point3(x + other.x, y + other.y, z + other.z);
  }
  bool operator==(const Point3& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_POINT3_