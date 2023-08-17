#include "time_it.hh"

#include <chrono>
#include <ctime>

// https://stackoverflow.com/a/27856440/5071723
uint32_t cs::profiling::time_it(
    std::function<void()> target) {
  auto start = std::chrono::system_clock::now();
  target();
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds =
      end - start;
  return static_cast<uint32_t>(elapsed_seconds.count() *
                               1000);
}