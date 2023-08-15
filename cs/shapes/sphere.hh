#ifndef CS_SHAPES_SPHERE_HH
#define CS_SHAPES_SPHERE_HH

#include <cmath>

#include "cs/geo/ray3.h"
#include "cs/geo/dot.hh"
#include "cs/string/format.h"
#include "cs/math/constants.h"

using r3 = ::cs::geo::Ray3;
using v3 = ::cs::geo::Vector3;
using p3 = ::cs::geo::Point3;
using ::cs::geo::dot;

namespace cs::shapes {
    struct Sphere {
        cs::geo::Point3 center;
        float radius;

        Sphere(p3 center, float radius): center(center), radius(radius) {

        }

        bool intersectedBy(const r3 ray, p3* atPoint, v3* atNormal) {
            float a = dot(ray.direction, ray.direction);
            float b = 2 * dot(ray.direction.b, (ray.origin - center));
            float c = dot((ray.origin - center), (ray.origin - center)) - radius * radius;

            // std::cout << cs::string::format("a = %6.4lf", a) << std::endl;
            
            if (!FLOAT_NEAR(a, 1.f)) {
                std::cout << "a != 1.0" << std::endl;
                return false;
            }

            float discriminant = b * b - 4 * c;
            // std::cout << cs::string::format("discriminant = %6.4lf", discriminant) << std::endl;
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
    };
} // namespace cs::shapes

#endif // CS_SHAPES_SPHERE_HH