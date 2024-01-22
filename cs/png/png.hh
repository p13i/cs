#ifndef CS_PNG_PNG_HH
#define CS_PNG_PNG_HH

#include <stdint.h>

namespace cs {
namespace png {

class PNG {
 public:
  uint64_t height, width;
};

PNG* MakePNG();
}  // namespace png
}  // namespace cs

#endif  // CS_PNG_PNG_HH