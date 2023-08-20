#ifndef CS_SANITY_ERROR_HH
#define CS_SANITY_ERROR_HH

#include <cstdlib>
#include <iostream>
#include <string>

#define ERROR(message) \
  cs::sanity::error(message, __FILE__, __LINE__)

namespace cs::sanity {
void error(std::string message, std::string file, int line);
}

#endif  // CS_SANITY_ERROR_HH