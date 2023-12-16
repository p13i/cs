#ifndef CS_RENDERER_SCENE_RENDERER_HH
#define CS_RENDERER_SCENE_RENDERER_HH

#include <stdio.h>

#include <tuple>
#include <vector>

#include "cs/geo/dist.hh"
#include "cs/geo/point3.h"
#include "cs/geo/ray3.h"
#include "cs/geo/vector3.h"
#include "cs/linalg/transform.hh"
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
using ::cs::linalg::Transform;
using ::cs::numbers::clamp;
using ::cs::numbers::map_value;
using ::cs::renderer::Film;
using ::cs::shapes::Sphere;

namespace cs::renderer {

struct Camera {
  Transform w2c_;
  unsigned int pixels_per_unit_;
  float focal_length_;
  Film film_;

  Camera(Transform w2c, unsigned int pixels_per_unit,
         float focal_length, Film film)
      : w2c_(w2c),
        pixels_per_unit_(pixels_per_unit),
        focal_length_(focal_length),
        film_(film) {
    ENSURE(pixels_per_unit_ > 0);
    ENSURE(focal_length_ > 0);
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Camera& camera) {
    return os << "Camera(pixels_per_unit="
              << camera.pixels_per_unit_
              << ", focal_length=" << camera.focal_length_
              << ", film dimensions: <"
              << std::get<0>(camera.film_.dimensions())
              << ", "
              << std::get<1>(camera.film_.dimensions())
              << ">)";
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