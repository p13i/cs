#ifndef CS_GEO_CROSS_HH
#define CS_GEO_CROSS_HH

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

namespace cs::geo {
v3 cross(v3 uno, v3 dos);
}  // namespace cs::geo

#endif  // CS_GEO_CROSS_HH