#ifndef CS_NUMBERS_IN_RANGE_HH
#define CS_NUMBERS_IN_RANGE_HH

namespace cs::numbers {
template <class T>
bool in_range(T value, T lower, T upper) {
  return lower <= value && value < upper;
}
}  // namespace cs::numbers

#endif  // CS_NUMBERS_IN_RANGE_HH
