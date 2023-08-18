#include "cs/renderer/scene.hh"

#include <limits>

#include "cs/geo/dist.hh"

bool cs::renderer::Scene::intersected_by(const r3 ray,
                                         p3* atPoint,
                                         v3* atNormal) {
  bool found_intersection = false;
  float min_distance = std::numeric_limits<float>::max();
  for (auto& shape : shapes_) {
    p3 this_intersection_point;
    v3 this_intersection_normal;
    if (shape->intersected_by(ray, &this_intersection_point,
                              &this_intersection_normal)) {
      float distance = cs::geo::dist(
          ray.origin, this_intersection_point);
      if (distance < min_distance) {
        found_intersection = true;
        min_distance = distance;
        *atPoint = this_intersection_point;
        *atNormal = this_intersection_normal;
      }
    }
  }
  return found_intersection;
}
