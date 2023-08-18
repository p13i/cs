#include "cs/shapes/sphere.hh"

#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

TEST(Sphere, IntersectionOnXAxis) {
  r3 ray(p3(0, 0, 0), p3(1, 0, 0));
  cs::shapes::Sphere sphere(p3(5, 0, 0), 0.5);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      sphere.intersectedBy(ray, &intersection, &normal));

  EXPECT_EQ(intersection, p3(4.5, 0, 0));
  EXPECT_EQ(normal, v3(p3(-1, 0, 0)));
}

TEST(Sphere, IntersectionOnYAxis) {
  r3 ray(p3(0, 0, 0), p3(0, 1, 0));
  cs::shapes::Sphere sphere(p3(0, 5, 0), 1);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      sphere.intersectedBy(ray, &intersection, &normal));

  EXPECT_EQ(intersection, p3(0, 4, 0));
  EXPECT_EQ(normal, v3(p3(0, -1, 0)));
}

TEST(Sphere, IntersectionOnZAxis) {
  r3 ray(p3(0, 0, 0), p3(0, 0, 1));
  cs::shapes::Sphere sphere(p3(0, 0, 5), 1);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      sphere.intersectedBy(ray, &intersection, &normal));

  EXPECT_EQ(intersection, p3(0, 0, 4));
  EXPECT_EQ(normal, v3(p3(0, 0, -1)));
}

TEST(Sphere, IntersectionOnXYPlane) {
  r3 ray(p3(0, 0, 0), p3(1, 1, 0));
  cs::shapes::Sphere sphere(p3(5, 5, 0), 1);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      sphere.intersectedBy(ray, &intersection, &normal));

  EXPECT_EQ(intersection,
            p3(5 - sqrtf(2) / 2, 5 - sqrtf(2) / 2, 0));
  EXPECT_EQ(normal, v3(p3(-sqrtf(2) / 2,
                                       -sqrtf(2) / 2, 0)));
}

TEST(Sphere, IntersectionInQuadrantOne) {
  r3 ray(p3(0, 0, 0), p3(1, 1, 1));
  cs::shapes::Sphere sphere(p3(5, 5, 5), 1);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      sphere.intersectedBy(ray, &intersection, &normal));

  EXPECT_EQ(intersection,
            p3(5 - sqrtf(3) / 3, 5 - sqrtf(3) / 3,
               5 - sqrtf(3) / 3));
  EXPECT_EQ(normal,
            v3(p3(-sqrtf(3) / 3, -sqrtf(3) / 3,
                               -sqrtf(3) / 3)));
}
