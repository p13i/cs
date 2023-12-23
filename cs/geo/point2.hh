#ifndef CS_GEO_POINT2_
#define CS_GEO_POINT2_

#include <iostream>
#include <cmath>

#include "cs/precision/floats.hh"

using ::cs::precision::FloatsNear;

namespace cs::geo {

class Point2 {
 public:
  Point2() : Point2(0, 0) {}

  Point2(float xy) : Point2(xy, xy) {}

  Point2(float x, float y) : x(x), y(y) {}

  Point2 unit() const {
    Point2 this_ = *this;
    return this_ /
           sqrtf(this_.x * this_.x + this_.y * this_.y);
  }

  Point2 operator+(const Point2& other) const {
    return Point2(x + other.x, y + other.y);
  }

  bool operator==(const Point2& other) const {
    return FloatsNear(x, other.x) &&
           FloatsNear(y, other.y);
  }

  bool operator<(const Point2& other) const {
    return x < other.x && y < other.y;
  }

  bool operator<=(const Point2& other) const {
    return x <= other.x && y <= other.y;
  }

  Point2 operator-(Point2 other) const {
    return {x - other.x, y - other.y};
  }

  Point2 operator*(float scalar) const {
    return {x * scalar, y * scalar};
  }

  Point2 operator*(Point2 other) const {
    return {x * other.x, y * other.y};
  }

  Point2 operator/(float scalar) const {
    return {x / scalar, y / scalar};
  }

  Point2 operator/(Point2 other) const {
    return {x / other.x, y / other.y};
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Point2& point) {
    return os << "Point2(" << point.x << ", " << point.y
              << ")";
  }

  float x, y;
};

}  // namespace cs::geo

#endif  // CS_GEO_POINT2_