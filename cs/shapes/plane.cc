#include "cs/shapes/plane.hh"

#include "cs/geo/dot.hh"
#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/precision/floats.hh"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;
using ::cs::geo::dot;

#include "cs/sanity/ensure.hh"

bool cs::shapes::Plane::intersected_by(r3 ray, p3* atPoint,
                                       v3* atNormal) {
  v3 unit_normal(p3(a, b, c));  // p_n
  ENSURE(unit_normal.is_unit());
  ENSURE(ray.direction.is_unit());
  float v_d = dot(unit_normal, ray.direction);
#if 0
  std::cout << "v_d " << v_d << std::endl;
#endif
  if (FloatsNear(v_d, 0)) {
    // Parallel
    return false;
  }
  float v_0 = -1 * (dot(unit_normal.b, ray.origin) + d);
  float t = v_0 / v_d;
#if 0
  std::cout << "v_0 = " << v_0 << std::endl;
  std::cout << "t = " << t << std::endl;
#endif
  if (t < 0) {
    // Intersection behind ray origin
    return false;
  }
  *atPoint = ray(t);
  if (v_d > 0) {
    // Normal is facing away so flip it
    unit_normal = unit_normal * -1.f;
  }
  *atNormal = unit_normal;
  return true;
}
