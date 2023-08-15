#ifndef CS_SANITY_ENSURE_HH
#define CS_SANITY_ENSURE_HH

#include <cstdlib>
#include <iostream>

#define ENSURE(condition) \
  cs::sanity::ensure(condition, __FILE__, __LINE__)

namespace cs::sanity {
void ensure(bool condition, char* file, int line) {
  if (!condition) {
    std::cerr << "Runtime assertion failed at " << file
              << ":" << line << "!";
    std::abort();
  }
}
}  // namespace cs::sanity

#endif  // CS_SANITY_ENSURE_HH