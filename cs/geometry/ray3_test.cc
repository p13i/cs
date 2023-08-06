#include "cs/geometry/ray3.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geometry::Point3;
using v3 = ::cs::geometry::Vector3;
using r3 = ::cs::geometry::Ray3;

#define EXPECT_POINT3_EQ(a, b) \
  EXPECT_FLOAT_EQ(a.x, b.x);   \
  EXPECT_FLOAT_EQ(a.y, b.y);   \
  EXPECT_FLOAT_EQ(a.z, b.z);

#define EXPECT_VECTOR3_EQ(vecA, vecB) \
  EXPECT_POINT3_EQ(vecA.a, vecB.a);   \
  EXPECT_POINT3_EQ(vecA.b, vecB.b);

TEST(Ray3, DirectionVectorIsUnit) {
  r3 ray(p3(), p3(1, 1, 1));
  EXPECT_VECTOR3_EQ(ray.direction, ray.direction.unit());
  EXPECT_POINT3_EQ(ray.direction.a, p3());
  EXPECT_POINT3_EQ(ray.direction.b,
                   p3(0.57735026918962576450914878050196,
                      0.57735026918962576450914878050196,
                      0.57735026918962576450914878050196));
}
