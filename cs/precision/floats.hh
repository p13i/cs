#ifndef CS_PRECISION_FLOAT_HH
#define CS_PRECISION_FLOAT_HH

#include <cmath>

namespace cs::precision {
constexpr float kFloatEpsilon = 0.0001f;
inline bool FloatsNear(float a, float b,
                       float epsilon = kFloatEpsilon) {
  return std::fabs(a - b) < epsilon;
}
};  // namespace cs::precision

#endif  // CS_PRECISION_FLOAT_HH