#include "cs/geo/vector3.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;

TEST(Vector3, OneArgConstructor) {
  EXPECT_EQ(v3(p3(1, 1, 1)), v3(1, 1, 1));
}

TEST(Vector3, OperatorAdd) {
  EXPECT_EQ(v3(p3(1, 1, 1)) + v3(p3(1, 2, 3)),
            v3(p3(2, 3, 4)));
}

TEST(Vector3, OperatorSubtract) {
  EXPECT_EQ(v3(p3(1, 1, 1)) - v3(p3(1, 2, 3)),
            v3(p3(0, -1, -2)));
}

TEST(Vector3, MagnitudeTest) {
  // Inline the variables for the test case
  p3 a(1.0f, 2.0f, 3.0f);
  p3 b(4.0f, 5.0f, 6.0f);
  v3 vec(b - a);

  // Calculate the magnitude manually (sqrt((4-1)^2 +
  // (5-2)^2 + (6-3)^2) = sqrt(27) ≈ 5.196152)
  EXPECT_FLOAT_EQ(vec.magnitude(), 5.196152f);
}

TEST(Vector3, Unit) {
  EXPECT_EQ(v3(p3(1, 1, 1)).unit(),
            v3(p3(0.57735026918962576450914878050196,
                  0.57735026918962576450914878050196,
                  0.57735026918962576450914878050196)));
}
