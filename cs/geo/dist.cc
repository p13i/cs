#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

namespace cs::geo {

float dist(p3 a, p3 b) {
  p3 diff = b - a;
  float x2 = diff.x * diff.x;
  float y2 = diff.y * diff.y;
  float z2 = diff.z * diff.z;
  return sqrtf(x2 + y2 + z2);
}

}  // namespace cs::geo
