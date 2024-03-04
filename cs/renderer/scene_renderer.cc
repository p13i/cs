#include "cs/renderer/scene_renderer.hh"

#include <stdio.h>

#include <tuple>
#include <vector>

#include "cs/geo/dist.hh"
#include "cs/geo/dot.hh"
#include "cs/geo/point2.hh"
#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/linalg/transform.hh"
#include "cs/numbers/clamp.hh"
#include "cs/numbers/map_value.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/scene.hh"
#include "cs/shapes/shape.hh"

using p2 = ::cs::geo::Point2;
using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;
using ::cs::geo::dist;
using ::cs::geo::dot;
using ::cs::linalg::Transform;
using ::cs::numbers::clamp;
using ::cs::numbers::map_value;
using ::cs::renderer::Film;
using ::cs::shapes::Shape;

Film cs::renderer::SceneRenderer::render() {
  // Unpack dimensions
  auto [width, height] = camera_.film_.dimensions();
  float x_units = width / camera_.pixels_per_unit_;
  float y_units = height / camera_.pixels_per_unit_;
  Transform w2c = camera_.w2c_;
  Transform c2w = w2c.inverse();
  const p3 focal_point_in_world =
      c2w(p3(0, 0, -1 * camera_.focal_length_)).value();
  for (unsigned int film_x = 0; film_x < width; film_x++) {
    for (unsigned int film_y = 0; film_y < height;
         film_y++) {
      p2 film_point = map_value(
          p2(film_x, film_y), p2(0, 0), p2(width, height),
          p2(-1 * x_units / 2.f, y_units / 2.f),
          p2(x_units / 2.f, -1 * y_units / 2.f));
      p3 film_point_in_world =
          c2w(p3(film_point.x, film_point.y, 0)).value();
      r3 ray(focal_point_in_world, film_point_in_world);
      p3 intersection_point;
      v3 normal;
      if (scene_.intersected_by(ray, &intersection_point,
                                &normal)) {
        // Compute the luminance HACK
        float unit_dot_prod = dot(
            (c2w(p3(0, 0, 0)).value() - intersection_point)
                .unit(),
            normal.unit());
        float luminance =
            map_value<float>(unit_dot_prod, 0, 1, 0, 255);
        camera_.film_.pixels[film_x][film_y] =
            Pixel(luminance, luminance, luminance, 255);
      } else {
        // A non-intersected point is black
        camera_.film_.pixels[film_x][film_y] =
            Pixel(0, 0, 0, 0);
      }
    }
  }
  return camera_.film_;
}