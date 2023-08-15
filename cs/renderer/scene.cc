#include "cs/renderer/scene.hh"

bool cs::renderer::Scene::intersectedBy(const r3 ray,
                                        p3* atPoint,
                                        v3* atNormal) {
  for (Sphere shape : shapes_) {
    if (shape.intersectedBy(ray, atPoint, atNormal)) {
      return true;
    }
  }
  return false;
}
