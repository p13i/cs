#include "cs/geo/dot.h"

#include "cs/geo/point3.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using ::cs::geo::dot;

TEST(Dot, Basic) {
  EXPECT_EQ(dot(p3(1, 2, 3), p3(4, 5, 6)),
            1 * 4 + 2 * 5 + 3 * 6);
}
