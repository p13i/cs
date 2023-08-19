#include "cs/shapes/plane.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

using ::cs::shapes::Plane;

TEST(Plane, IntersectionOnXAxis) {
  r3 ray(p3(), p3(1, 0, 0));
  Plane plane(p3(1, 0, 0).unit(), -2);

  p3 intersection;
  v3 normal;
  EXPECT_TRUE(
      plane.intersected_by(ray, &intersection, &normal));

  EXPECT_EQ(intersection, p3(2, 0, 0));
  EXPECT_EQ(normal, v3(p3(), p3(-1, 0, 0)));
}
