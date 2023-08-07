#include "cs/geo/dist.h"

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
