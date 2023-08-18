#ifndef CS_RENDERER_SCENE_HH
#define CS_RENDERER_SCENE_HH

#include <vector>

#include "cs/shapes/shape.hh"

using ::cs::shapes::Shape;

namespace cs::renderer {
class Scene {
 public:
  std::vector<Shape*> shapes_;
  Scene() : Scene(std::vector<Shape*>()) {}
  Scene(std::vector<Shape*> shapes) : shapes_(shapes) {}
  bool intersected_by(const r3 ray, p3* atPoint,
                     v3* atNormal);
};
}  // namespace cs::renderer

#endif  // CS_RENDERER_SCENE_HH