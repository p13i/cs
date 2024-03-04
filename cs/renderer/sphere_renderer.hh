#ifndef CS_RENDERER_SPHERE_RENDERER_HH
#define CS_RENDERER_SPHERE_RENDERER_HH

#include <stdio.h>

#include <vector>

#include "cs/geo/dist.hh"
#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/numbers/map_value.hh"
#include "cs/renderer/film.hh"
#include "cs/shapes/sphere.hh"
#include "cs/result/result.hh"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;
using ::cs::geo::dist;
using ::cs::numbers::map_value;
using ::cs::renderer::Film;
using ::cs::shapes::Sphere;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;

namespace cs::renderer {
class SphereRenderer {
 public:
  p3 focal_point_ = p3(0, 0, -2);
  p3 film_center_ = p3(0, 0, 1);
  Film film_ = Film(256, 256);
  float pixels_per_unit_ = 128;
  Sphere sphere_ =
      Sphere(/*center=*/p3(0, 0, 5), /*radius=*/1);

  ResultOr<Film> render() {
    float min_distance =
        dist(focal_point_, sphere_.center) - sphere_.radius;
    float max_distance =
        dist(focal_point_, sphere_.center) + sphere_.radius;

    float x_units = film_.width / pixels_per_unit_;
    float y_units = film_.height / pixels_per_unit_;
    float film_x_start = film_center_.x - x_units / 2.f;
    float film_x_end = film_center_.x + x_units / 2.f;
    float film_y_start = film_center_.y + y_units / 2.f;
    float film_y_end = film_center_.y - y_units / 2.f;
    for (uint32_t fx = 0; fx < film_.width; fx++) {
      float real_x = map_value<float>(
          fx, 0, film_.width, film_x_start, film_x_end);
      for (uint32_t fy = 0; fy < film_.height; fy++) {
        float real_y = map_value<float>(
            fy, 0, film_.height, film_y_start, film_y_end);
        float real_z = film_center_.z;
        r3 ray(focal_point_, p3(real_x, real_y, real_z));

        p3 intersection_point;
        v3 normal;
        if (sphere_.intersected_by(ray, &intersection_point,
                                   &normal)) {
          float distance =
              cs::geo::dist(ray.origin, intersection_point);
#if 0
          if (distance > max_distance) {
            max_distance = distance;
            std::cout
                << "Film point fx=" << fx << ",fy=" << fx
                << " max_distance=" << max_distance << "\n";
          }
#endif
          if (!(distance >= min_distance)) {
            return Error("distance < min_distance");
          }
          if (!(distance <= max_distance)) {
            return Error("distance > max_distance");
          }
          float rgb = map_value<float>(
              distance, min_distance, max_distance, 255, 0);
          if (!(0 <= rgb && rgb <= 255.f)) {
            return Error("rgb out of bounds");
          }
          film_.pixels[fx][fy] = Pixel(rgb, rgb, rgb, 255);
        } else {
          film_.pixels[fx][fy] = Pixel(0, 0, 0, 0);
        }
      }
    }
    return film_;
  }
};
}  // namespace cs::renderer

#endif  // CS_RENDERER_SPHERE_RENDERER_HH