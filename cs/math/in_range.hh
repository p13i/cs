#ifndef CS_MATH_IN_RANGE_HH
#define CS_MATH_IN_RANGE_HH

namespace cs::math {
template <class T>
bool in_range(T value, T lower, T upper) {
  return lower <= value && value < upper;
}
}  // namespace cs::math

#endif  // CS_MATH_IN_RANGE_HH
