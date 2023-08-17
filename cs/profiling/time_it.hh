#ifndef CS_PROFILING_TIME_IT
#define CS_PROFILING_TIME_IT

#include <time.h>
#include <functional>
#include <stdint.h>

namespace cs::profiling {

uint32_t time_it(std::function<void()> target);

}  // namespace cs::profiling

#endif  // CS_PROFILING_TIME_IT