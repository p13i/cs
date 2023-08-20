#include "cs/shapes/sphere.hh"

#include <cmath>

#include "cs/geo/dot.hh"
#include "cs/geo/ray3.h"
#include "cs/precision/floats.hh"
#include "cs/shapes/shape.hh"
#include "cs/string/format.h"

using r3 = ::cs::geo::Ray3;
using v3 = ::cs::geo::Vector3;
using p3 = ::cs::geo::Point3;
using ::cs::geo::dot;
using ::cs::precision::FloatsNear;

bool cs::shapes::Sphere::intersected_by(r3 ray, p3* atPoint,
                                        v3* atNormal) {
  float a = dot(ray.direction, ray.direction);
  float b = 2 * dot(ray.direction, (ray.origin - center));
  float c =
      dot((ray.origin - center), (ray.origin - center)) -
      radius * radius;

  // std::cout << cs::string::format("a = %6.4lf", a) <<
  // std::endl;

  if (!FloatsNear(a, 1.f)) {
    std::cout << "a != 1.0" << std::endl;
    return false;
  }

  float discriminant = b * b - 4 * c;
  // std::cout << cs::string::format("discriminant =
  // %6.4lf", discriminant) << std::endl;
  if (discriminant < 0.f) {
    return false;
  }

  float t_intersect;
  float t0 = (-b + sqrtf(discriminant)) / 2.f;
  float t1 = (-b - sqrtf(discriminant)) / 2.f;
  // Intersection time must be the positve solution
  if (t0 < t1) {
    t_intersect = t0;
  } else {
    t_intersect = t1;
  }

  p3 intersection = ray(t_intersect);
  v3 normal = ((intersection - center) / radius);
  normal = normal.unit();

  *atPoint = intersection;
  *atNormal = normal;
  return true;
};
