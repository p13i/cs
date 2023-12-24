#ifndef CS_NUMBERS_MAP_VALUE_HH
#define CS_NUMBERS_MAP_VALUE_HH

namespace cs::numbers {
template <class T>
T map_value(T value, T value_range_start, T value_range_end,
            T new_range_start, T new_range_end) {
  return new_range_start +
         ((new_range_end - new_range_start) /
          (value_range_end - value_range_start)) *
             (value - value_range_start);
};

}  // namespace cs::numbers

#endif  // CS_NUMBERS_MAP_VALUE_HH
