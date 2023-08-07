#ifndef CS_GEO_RAY3_
#define CS_GEO_RAY3_

#include <utility>

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"
#include "cs/math/polar2cart.h"

namespace cs::geo {

// An unbounded vector
struct Ray3 {
  Point3 origin;
  Vector3 direction;

  // Empty constructor
  Ray3() : Ray3(Point3(), Point3()) {}

  // Uses physics convention, ISO 80000-2:2019. See:
  // https://en.wikipedia.org/wiki/Spherical_coordinate_system
  Ray3(const Point3& origin, float theta, float phi)
      : Ray3(origin, Vector3(origin, cs::math::polar2cart(
                                         1, theta, phi))) {}

  // A ray from an origin in the direction towards a point
  Ray3(const Point3& origin, const Point3& towards)
      : Ray3(origin, Vector3(towards - origin)) {}

  // A ray with a given origin and destination
  Ray3(const Point3& origin, const Vector3& direction)
      : origin(origin), direction(direction.unit()) {}

  // Converting a vector directly to a ray where the
  // vector's origin is the origin of the ray
  explicit Ray3(const Vector3& vector)
      : Ray3(vector.a, vector.b) {}

  // Evaluates a ray at a given time-step multiplied against
  // the unit vector direction
  Point3 operator()(float time) const {
    const Vector3 unit = direction.unit();
    return origin + unit.b * time;
  };

  friend std::ostream& operator<<(std::ostream& os,
                                  const Ray3& ray) {
    os << "Ray3(" << ray.origin << ", " << ray.direction
       << ")";
    return os;
  }
};

}  // namespace cs::geo

#endif  // CS_GEO_RAY3_