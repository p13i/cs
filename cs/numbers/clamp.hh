#ifndef CS_NUMBERS_CLAMP_HH
#define CS_NUMBERS_CLAMP_HH

namespace cs::numbers {
template <class T>
T clamp(T value, T min_value, T max_value) {
  if (value < min_value) {
    return min_value;
  }
  if (value > max_value) {
    return max_value;
  }
  return value;
}
}  // namespace cs::numbers

#endif  // CS_NUMBERS_CLAMP_HH