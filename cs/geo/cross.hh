#ifndef CS_GEO_CROSS_HH
#define CS_GEO_CROSS_HH

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

namespace cs::geo {
v3 cross(v3 uno, v3 dos) {
  // Each vector must just be a direction vector
  // Start the point at the origin
  uno = uno - uno.a;
  dos = dos - dos.a;
  // Make the vector have unit magnitude
  uno = uno.unit();
  dos = dos.unit();

  p3 a = uno.b;
  p3 b = dos.b;

  float cx = a.y * b.z - a.z * b.y;
  float cy = a.z * b.x - a.x * b.z;
  float cz = a.x * b.y - a.y * b.x;

  return v3(p3(cx, cy, cz));
}
}  // namespace cs::geo

#endif  // CS_GEO_CROSS_HH