#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "cs/geometry/point3.h" 

using p3 = ::cs::geometry::Point3;

TEST(Point3, OperatorAdd) {
    p3 a{1, 2, 3};
    p3 b{4, 5, 6};
    p3 expected{5,7,9};
    EXPECT_EQ(a + b, expected);
}
