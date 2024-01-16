#include "cs/png/crc.hh"

// From http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html#CRC-algorithm
uint64_t cs::crc::ComputeCRC(uint8_t* buffer,
                             uint64_t length) {
  uint64_t crc_table[256];

  // Populate CRC table
  unsigned long c;
  int n, k;

  for (n = 0; n < 256; n++) {
    c = (unsigned long)n;
    for (k = 0; k < 8; k++) {
      if (c & 1)
        c = 0xedb88320L ^ (c >> 1);
      else
        c = c >> 1;
    }
    crc_table[n] = c;
  }

  // Compute CRC
  c = 0xffffffffL;
  n = 0;
  for (n = 0; n < length; n++) {
    c = crc_table[(c ^ buffer[n]) & 0xff] ^ (c >> 8);
  }

  return c ^ 0xffffffffL;
}
