#ifndef CS_RENDERER_SCENE_HH
#define CS_RENDERER_SCENE_HH

#include <vector>

#include "cs/shapes/sphere.hh"

using ::cs::shapes::Sphere;

namespace cs::renderer {
class Scene {
 public:
  std::vector<Sphere> shapes_;
  Scene() : Scene(std::vector<Sphere>()) {}
  Scene(std::vector<Sphere> shapes) : shapes_(shapes) {}
  bool intersectedBy(const r3 ray, p3* atPoint,
                     v3* atNormal);
};
}  // namespace cs::renderer

#endif  // CS_RENDERER_SCENE_HH