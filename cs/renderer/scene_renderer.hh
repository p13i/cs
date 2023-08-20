#ifndef CS_RENDERER_SCENE_RENDERER_HH
#define CS_RENDERER_SCENE_RENDERER_HH

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

namespace cs::renderer {

struct Camera {
  p3 focal_point_;
  p3 film_center_;
  unsigned int pixels_per_unit_;
  Film film_;

  Camera(p3 focal_point, p3 film_center,
         unsigned int pixels_per_unit,
         std::tuple<unsigned int, unsigned int> film_dimensions)
      : focal_point_(focal_point),
        film_center_(film_center),
        pixels_per_unit_(pixels_per_unit),
        film_(Film(film_dimensions)) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const Camera& camera) {
    return os << "Camera(focal_point="
              << camera.focal_point_
              << ", film_center=" << camera.film_center_
              << ", pixels_per_unit="
              << camera.pixels_per_unit_
              << ", film dimensions: <"
              << std::get<0>(camera.film_.dimensions()) << ", " << std::get<1>(camera.film_.dimensions()) << ">)";
  }
};

class SceneRenderer {
 public:
  Camera camera_;
  Scene scene_;

  SceneRenderer(Camera camera, Scene scene)
      : camera_(camera), scene_(scene) {}

  Film render();
};
}  // namespace cs::renderer

#endif  // CS_RENDERER_SPHERE_RENDERER_HH