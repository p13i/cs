#include "cs/math/map_value.hh"

float cs::math::map_value(float value,
                          float value_range_start,
                          float value_range_end,
                          float new_range_start,
                          float new_range_end) {
  return new_range_start +
         ((new_range_end - new_range_start) /
          (value_range_end - value_range_start)) *
             (value - value_range_start);
};