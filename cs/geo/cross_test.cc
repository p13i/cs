#include "cs/geo/cross.hh"

#include "cs/geo/point3.h"
#include "cs/geo/vector3.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using ::cs::geo::cross;

TEST(CrossProduct, CoordinateAxes) {
  EXPECT_EQ(cross(v3(p3(1, 0, 0)), v3(p3(0, 1, 0))),
            v3(p3(0, 0, 1)));
}
