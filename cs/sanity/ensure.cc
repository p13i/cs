#include "cs/sanity/ensure.hh"

void cs::sanity::ensure(bool condition,
                        std::string condition_literal,
                        std::string file, int line) {
  if (!condition) {
    std::cerr << "Runtime assertion (" << condition_literal
              << ") failed at " << file << ":" << line
              << "!" << std::endl;
    std::abort();
  }
}
