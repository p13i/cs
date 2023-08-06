#ifndef CS_GEOMETRY_RAY3_
#define CS_GEOMETRY_RAY3_

#include <utility>

#include "cs/geometry/point3.h"
#include "cs/geometry/vector3.h"

namespace cs::geometry {

struct Angles {
  // Radians
  float azimuth = 0;
  // Radians
  float elevation = 0;
  Angles(float azimuth, float elevation)
      : azimuth(azimuth), elevation(elevation) {}
};

// A unbounded vector
struct Ray3 {
  Point3 origin;
  Vector3 direction;

  // Empty constructor
  Ray3() : Ray3(Point3(), Point3()) {}

  Ray3(const Point3& origin, Angles angles)
      : origin(origin),
        direction(Vector3(
            Point3(std::cos(angles.azimuth) *
                       std::cos(angles.elevation),
                   std::sin(angles.elevation),
                   std::sin(angles.azimuth) *
                       std::cos(angles.elevation)))) {}

  // A ray from an origin in the direction towards a point
  Ray3(const Point3 origin, const Point3 towards)
      : Ray3(origin, Vector3(towards - origin)) {}

  // A ray with a given origin and destination
  Ray3(const Point3 origin, const Vector3 direction)
      : origin(origin), direction(direction.unit()) {}

  // Converting a vector directly to a ray where the
  // vector's origin is the origin of the ray
  explicit Ray3(const Vector3 vector)
      : Ray3(vector.a, vector.b) {}

  // Evaluates a ray at a given time-step multiplied against
  // the unit vector direction
  Point3 operator()(float time) const {
    const Vector3 unit = direction.unit();
    return origin + unit.b * time;
  };

  Angles angles() {
    const Point3 point = direction.b;
    return Angles(
        std::atan2(point.y, point.x),
        std::atan2(point.z, std::sqrt(point.x * point.x +
                                      point.y * point.y)));
  };
};

}  // namespace cs::geometry

#endif  // CS_GEOMETRY_RAY3_