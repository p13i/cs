#ifndef CS_SANITY_ENSURE_HH
#define CS_SANITY_ENSURE_HH

#include <cstdlib>
#include <iostream>
#include <string>

#define ENSURE(condition)                             \
  cs::sanity::ensure(condition, #condition, __FILE__, \
                     __LINE__)

namespace cs::sanity {
void ensure(bool condition, std::string condition_literal,
            std::string file, int line);
}  // namespace cs::sanity

#endif  // CS_SANITY_ENSURE_HH