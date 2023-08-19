#include "cs/geo/ray3.h"

#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

TEST(Ray3, DirectionVectorIsUnit) {
  r3 ray(p3(), p3(1, 1, 1));
  EXPECT_EQ(ray.direction, ray.direction.unit());
  EXPECT_EQ(ray.direction.a, p3());
  EXPECT_EQ(ray.direction.b,
            p3(0.57735026918962576450914878050196,
               0.57735026918962576450914878050196,
               0.57735026918962576450914878050196));
}

TEST(Ray3, OperatorTimeOneComponent) {
  r3 ray(p3(), p3(1, 0, 0));
  EXPECT_EQ(ray(3), p3(3, 0, 0));
}

TEST(Ray3, OperatorTimeOnUnitVector) {
  r3 ray(p3(), p3(1, 1, 1));
  EXPECT_EQ(ray(3), p3(1.7320508075688772935274463415059,
                       1.7320508075688772935274463415059,
                       1.7320508075688772935274463415059));
}

TEST(Ray3, ThetaIsPiOver2) {
  r3 ray(p3(), PIf / 2.f, 0);
  v3 expected(p3(), p3(1, 0, 0));
  EXPECT_EQ(ray.direction, expected);
}

TEST(Ray3, ThetaIsPiOver4) {
  r3 ray(p3(), PIf / 4.f, 0);
  v3 expected(p3(), p3(sqrtf(2) / 2.f, 0, sqrtf(2) / 2.f));
  EXPECT_EQ(ray.direction, expected);
}

TEST(Ray3, PiOver2PhiStillPointsToZ) {
  r3 ray(p3(), 0, PIf / 2.f);
  v3 expected(p3(), p3(0, 0, 1));
  EXPECT_EQ(ray.direction, expected);
}

TEST(Ray3, ThetaAndPhiArePiOver2) {
  r3 ray(p3(), PIf / 2.f, PIf / 2.f);
  v3 expected(p3(), p3(0, 1, 0));
  EXPECT_EQ(ray.direction, expected);
}

TEST(Ray3, ThetaAndPhiArePiOver4) {
  r3 ray(p3(), PIf / 4.f, PIf / 4.f);
  v3 expected(p3(), p3(1 / 2.f, 1 / 2.f, sqrtf(2) / 2.f));
  EXPECT_EQ(ray.direction, expected);
}
