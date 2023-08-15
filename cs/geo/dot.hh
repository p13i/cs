#ifndef CS_GEO_DOT_H
#define CS_GEO_DOT_H

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

namespace cs::geo {
// Performs a standard dot product.
float dot(const p3& a, const p3& b);
float dot(const v3& a, const v3& b);
}  // namespace cs::geo

#endif  // CS_GEO_DOT_H