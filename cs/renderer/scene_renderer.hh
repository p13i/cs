#ifndef CS_RENDERER_SCENE_RENDERER_HH
#define CS_RENDERER_SCENE_RENDERER_HH

#include <stdio.h>

#include <tuple>
#include <vector>

#include "cs/collections/tuple.hh"
#include "cs/geo/dist.hh"
#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/numbers/clamp.hh"
#include "cs/numbers/map_value.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/scene.hh"
#include "cs/sanity/ensure.hh"
#include "cs/shapes/sphere.hh"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;
using ::cs::collections::Tuple;
using ::cs::geo::dist;
using ::cs::numbers::clamp;
using ::cs::numbers::map_value;
using ::cs::renderer::Film;
using ::cs::shapes::Sphere;

namespace cs::renderer {

struct Camera {
  p3 focal_point_;
  p3 film_center_;
  uint32_t pixels_per_unit_;
  Film film_;

  Camera(p3 focal_point, p3 film_center,
         uint32_t pixels_per_unit,
         Tuple<uint32_t, uint32_t> film_dimensions)
      : focal_point_(focal_point),
        film_center_(film_center),
        pixels_per_unit_(pixels_per_unit),
        film_(Film(film_dimensions)) {}
};

class SceneRenderer {
 public:
  Camera camera_;
  Scene scene_;

  SceneRenderer(Camera camera, Scene scene)
      : camera_(camera), scene_(scene) {}

  Film render() {
    float x_units =
        camera_.film_.width / camera_.pixels_per_unit_;
    float y_units =
        camera_.film_.height / camera_.pixels_per_unit_;
    float film_x_start =
        camera_.film_center_.x - x_units / 2.f;
    float film_x_end =
        camera_.film_center_.x + x_units / 2.f;
    float film_y_start =
        camera_.film_center_.y + y_units / 2.f;
    float film_y_end =
        camera_.film_center_.y - y_units / 2.f;
    for (uint32_t fx = 0; fx < camera_.film_.width; fx++) {
      float real_x =
          map_value<float>(fx, 0, camera_.film_.width,
                           film_x_start, film_x_end);
      for (uint32_t fy = 0; fy < camera_.film_.height;
           fy++) {
        float real_y =
            map_value<float>(fy, 0, camera_.film_.height,
                             film_y_start, film_y_end);
        float real_z = camera_.film_center_.z;
        r3 ray(camera_.focal_point_,
               p3(real_x, real_y, real_z));

#if 0
        std::cout << "Ray at " << Tuple(fx, fy) << " is "
                  << ray << std::endl;
#endif

        p3 intersection_point;
        v3 normal;
        if (scene_.intersectedBy(ray, &intersection_point,
                                 &normal)) {
          float unit_dot_prod = dot(
              v3(intersection_point, camera_.film_center_)
                  .unit(),
              normal.unit());
          float luminance =
              map_value<float>(unit_dot_prod, 0, 1, 0, 255);
          camera_.film_.pixels[fx][fy] =
              Pixel(luminance, luminance, luminance, 255);
        } else {
          camera_.film_.pixels[fx][fy] = Pixel(0, 0, 0, 0);
        }
      }
    }
    return camera_.film_;
  }
};
}  // namespace cs::renderer

#endif  // CS_RENDERER_SPHERE_RENDERER_HH