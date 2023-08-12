#include "cs/audio/sinusodal.h"

#include <cmath>

#include "cs/math/constants.h"

float cs::audio::sinusodal(
    float freqencyHz, float phase) {
  return static_cast<float>(sinf(
      freqencyHz * 2.f * PIf * phase));
};