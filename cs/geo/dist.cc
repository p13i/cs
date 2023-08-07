#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

namespace cs::geo {

float dist(Point3 a, Point3 b) {
  return Vector3(a, b).magnitude();
}

}  // namespace cs::geo
