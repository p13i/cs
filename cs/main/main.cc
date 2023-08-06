#include <iostream>

#include "cs/geometry/point3.h"
#include "cs/geometry/vector3.h"

using p3 = ::cs::geometry::Point3;
using v3 = ::cs::geometry::Vector3;

int main() {
  v3 vec(p3(1, 2, 3), p3(4, 5, 6));
  std::cout << vec << std::endl;
  return EXIT_SUCCESS;
}
