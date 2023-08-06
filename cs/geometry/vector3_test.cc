#include "cs/geometry/vector3.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geometry::Point3;
using v3 = ::cs::geometry::Vector3;

TEST(Vector3, OperatorAdd) {
  EXPECT_EQ(v3(p3(0, 0, 0), p3(1, 1, 1)).a, p3(0, 0, 0));
}
