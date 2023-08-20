#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

namespace cs::geo {

float dist(p3 a, p3 b) {
  p3 diff = b - a;
  return sqrtf(diff.x * diff.x + diff.y * diff.y +
               diff.z * diff.z);
}

}  // namespace cs::geo
