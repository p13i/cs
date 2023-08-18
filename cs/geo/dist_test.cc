#include "cs/geo/dist.hh"

#include "cs/geo/point3.h"
#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using ::cs::geo::dist;

TEST(Dist, Basic) {
  EXPECT_NEAR(dist(p3(), p3(1, 1, 1)), sqrtf(3),
              FLOAT_EPSILON);
}

// See:
// https://www.wolframalpha.com/input?i=distance+from+%28-1%2C2.5%2C3.5%29+to+%28-5%2C2.5%2C-7.5%29
TEST(Dist, Basic2) {
  EXPECT_NEAR(dist(p3(-1, 2.5, 3.5), p3(-5, 2.5, -7.5)),
              11.7047, FLOAT_EPSILON);
}
