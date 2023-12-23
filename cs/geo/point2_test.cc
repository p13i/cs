#include "cs/geo/point2.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p2 = ::cs::geo::Point2;

TEST(Point2, OperatorAdd) {
  EXPECT_EQ(p2(1, 2) + p2(4, 5), p2(5, 7));
}

TEST(Point3, OperatorSubtract) {
  EXPECT_EQ(p2(1, 2) - p2(4, 5), p2(-3, -3));
}

TEST(Point3, OperatorMultiply) {
  EXPECT_EQ(p2(1, 2) * 3, p2(3, 6));
}

TEST(Point3, OperatorDivide) {
  EXPECT_EQ(p2(1, 2) / 3, p2(1 / 3.f, 2 / 3.f));
}
