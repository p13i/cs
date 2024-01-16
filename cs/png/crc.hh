#include <stdint.h>

#ifndef CS_CRC_HH
#define CS_CRC_HH

namespace cs {
namespace crc {

uint64_t ComputeCRC(uint8_t* buffer, uint64_t length);

}
}  // namespace cs

#endif  // CS_CRC_HH