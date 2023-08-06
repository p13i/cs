#include "cs/geometry/point3.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geometry::Point3;

TEST(Point3, OperatorAdd) {
  EXPECT_EQ(p3(1, 2, 3) + p3(4, 5, 6), p3(5, 7, 9));
}
