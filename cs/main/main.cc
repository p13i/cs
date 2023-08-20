#include <iostream>

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

int main() {
  v3 vec(p3(4, 5, 6) - p3(1, 2, 3));
  std::cout << vec << std::endl;
  return EXIT_SUCCESS;
}
