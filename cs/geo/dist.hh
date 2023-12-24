#ifndef CS_GEO_DIST_HH
#define CS_GEO_DIST_HH

#include "cs/geo/point3.h"

namespace cs::geo {

using p3 = ::cs::geo::Point3;

float dist(p3 a, p3 b);

}  // namespace cs::geo

#endif  // CS_GEO_DIST_HH