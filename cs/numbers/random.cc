#include "random.hh"

#include <random>

float cs::math::randomFloat() {
  return std::rand() / static_cast<float>(RAND_MAX);
};
