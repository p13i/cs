#include "cs/math/map_value.hh"

#include "cs/math/constants.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::math::map_value;

TEST(MapValueTest, TestInRange) {
  float value = 50.0f;
  float value_range_start = 0.0f;
  float value_range_end = 100.0f;
  float new_range_start = 10.0f;
  float new_range_end = 200.0f;

  float mapped_value =
      map_value(value, value_range_start, value_range_end,
                new_range_start, new_range_end);

  EXPECT_NEAR(mapped_value, 105.0f, FLOAT_EPSILON);
}

TEST(MapValueTest, TestOutOfRange) {
  float value = 150.0f;
  float value_range_start = 0.0f;
  float value_range_end = 100.0f;
  float new_range_start = 10.0f;
  float new_range_end = 200.0f;

  float mapped_value =
      map_value(value, value_range_start, value_range_end,
                new_range_start, new_range_end);

  EXPECT_NEAR(mapped_value, 295.f, FLOAT_EPSILON);
}
