#include "cs/geo/ray3.h"

#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

#define EXPECT_POINT3_EQ(a, b)    \
  EXPECT_NEAR(a.x, b.x, EPSILON); \
  EXPECT_NEAR(a.y, b.y, EPSILON); \
  EXPECT_NEAR(a.z, b.z, EPSILON);

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

TEST(Ray3, OperatorTimeOneComponent) {
  r3 ray(p3(), p3(1, 0, 0));
  EXPECT_POINT3_EQ(ray(3), p3(3, 0, 0));
}

TEST(Ray3, OperatorTimeOnUnitVector) {
  r3 ray(p3(), p3(1, 1, 1));
  EXPECT_POINT3_EQ(ray(3),
                   p3(1.7320508075688772935274463415059,
                      1.7320508075688772935274463415059,
                      1.7320508075688772935274463415059));
}

TEST(Ray3, Angles) {
  r3 ray(p3(), PIf / 2.f, 0);
  v3 expected(p3(), p3(1, 0, 0));
  EXPECT_VECTOR3_EQ(ray.direction, expected);
}
