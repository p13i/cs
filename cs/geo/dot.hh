#ifndef CS_GEO_DOT_H
#define CS_GEO_DOT_H

#include "cs/geo/point3.h"

namespace cs::geo {
// Performs a standard dot product.
float dot(const Point3& a, const Point3& b);
}  // namespace cs::geo

#endif  // CS_GEO_DOT_H