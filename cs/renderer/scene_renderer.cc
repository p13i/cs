#include "cs/renderer/scene_renderer.hh"

#include <stdio.h>

#include <tuple>
#include <vector>

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
using ::cs::geo::dist;
using ::cs::numbers::clamp;
using ::cs::numbers::map_value;
using ::cs::renderer::Film;
using ::cs::shapes::Sphere;

Film cs::renderer::SceneRenderer::render() {
  // Unpack dimensions
  auto [width, height] = camera_.film_.dimensions();
  float x_units = width / camera_.pixels_per_unit_;
  float y_units = height / camera_.pixels_per_unit_;
  // Unpack freqently-used variables
  p3 film_center = camera_.film_center_;
  // Compute bounds of film plane to sample
  p3 film_start = film_center +
                  p3(-1 * x_units / 2.f, y_units / 2.f, 0);
  p3 film_end = film_center +
                p3(x_units / 2.f, -1 * y_units / 2.f, 0);
  // Compute each pixel value
  for (unsigned int fx = 0; fx < width; fx++) {
    for (unsigned int fy = 0; fy < height; fy++) {
      // Compute ray projected from focal point through film
      // point
      p3 real_p = map_value<p3>(p3(fx, fy, 0), p3(0, 0, 0),
                                p3(width, height, 0),
                                film_start, film_end);
      real_p.z = film_center.z;
      r3 ray(camera_.focal_point_, real_p);

      // Find intersection from this pixel
      p3 intersection_point;
      v3 normal;
      if (scene_.intersected_by(ray, &intersection_point,
                                &normal)) {
        // Compute the luminance of a pixel with an
        // intersection
        float unit_dot_prod =
            dot(v3(intersection_point, film_center).unit(),
                normal.unit());
        float luminance =
            map_value<float>(unit_dot_prod, 0, 1, 0, 255);
        camera_.film_.pixels[fx][fy] =
            Pixel(luminance, luminance, luminance, 255);
      } else {
        // A non-intersected point is black
        camera_.film_.pixels[fx][fy] = Pixel(0, 0, 0, 0);
      }
    }
  }
  return camera_.film_;
}