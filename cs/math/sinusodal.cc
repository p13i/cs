#include "cs/math/sinusodal.h"

#include <cmath>

#include "cs/math/constants.h"

float cs::math::sinusodal(float freqencyHz, float phase) {
  return static_cast<float>(
      sinf(freqencyHz * 2.f * PIf * phase));
};