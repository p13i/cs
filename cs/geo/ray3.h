#ifndef CS_GEO_RAY3_
#define CS_GEO_RAY3_

#include <utility>

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"
#include "cs/math/polar2cart.hh"

namespace cs::geo {

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

// An unbounded vector
struct Ray3 {
  p3 origin;
  v3 direction;

  // Empty constructor
  Ray3() : Ray3(p3(), p3()) {}

  // Uses physics convention, ISO 80000-2:2019. See:
  // https://en.wikipedia.org/wiki/Spherical_coordinate_system
  Ray3(const p3& origin, float theta, float phi)
      : Ray3(origin, (origin + cs::math::polar2cart(
                                   1, theta, phi))) {}

  // A ray from an origin in the direction towards a point
  Ray3(const p3& origin, const p3& towards)
      : Ray3(origin, v3(towards - origin)) {}

  // A ray with a given origin and destination
  Ray3(const p3& origin, const v3& direction)
      : origin(origin), direction(direction.unit()) {}

  // Evaluates a ray at a given time-step multiplied against
  // the unit vector direction
  p3 operator()(float time) const {
    v3 unit = direction.unit();
    p3 unit_p(unit.x, unit.y, unit.z);
    return origin + unit_p * time;
  };

  friend std::ostream& operator<<(std::ostream& os,
                                  const Ray3& ray) {
    os << "Ray3(origin=" << ray.origin
       << ", direction=" << ray.direction << ")";
    return os;
  }
};

}  // namespace cs::geo

#endif  // CS_GEO_RAY3_