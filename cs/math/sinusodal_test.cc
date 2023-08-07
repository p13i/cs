#include "cs/math/sinusodal.h"

#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::math::sinusodal;

TEST(Sinusodal, Zero) {
  EXPECT_NEAR(sinusodal(440, 0), 0, FLOAT_EPSILON);
}

TEST(Sinusodal, HalfPhase) {
  EXPECT_NEAR(sinusodal(440, 0.5), 0, FLOAT_EPSILON);
}
