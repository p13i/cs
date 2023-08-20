#include "cs/sanity/error.hh"

void cs::sanity::error(std::string message,
                       std::string file, int line) {
  std::cerr << "Runtime assertion failed at " << file << ":"
            << line << ": " << message << std::endl;
  std::abort();
}
