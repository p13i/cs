#ifndef CS_MATH_MAP_VALUE_HH
#define CS_MATH_MAP_VALUE_HH

namespace cs::math {
float map_value(float value, float value_range_start,
                float value_range_end,
                float new_range_start, float new_range_end);
}  // namespace cs::math

#endif  // CS_MATH_MAP_VALUE_HH