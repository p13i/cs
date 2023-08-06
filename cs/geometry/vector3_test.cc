#include "cs/geometry/vector3.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geometry::Point3;
using v3 = ::cs::geometry::Vector3;

TEST(Vector3, OperatorAdd) {
  EXPECT_EQ(v3(p3(0, 0, 0), p3(1, 1, 1)).a, p3(0, 0, 0));
}

TEST(Vector3, MagnitudeTest) {
  // Inline the variables for the test case
  p3 a(1.0f, 2.0f, 3.0f);
  p3 b(4.0f, 5.0f, 6.0f);
  v3 vec(a, b);

  // Calculate the magnitude manually (sqrt((4-1)^2 +
  // (5-2)^2 + (6-3)^2) = sqrt(27) ≈ 5.196152)
  EXPECT_FLOAT_EQ(vec.magnitude(), 5.196152f);
}
