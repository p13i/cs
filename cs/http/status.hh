#ifndef CS_HTTP_STATUS_HH
#define CS_HTTP_STATUS_HH

#include <stdint.h>

#include <map>
#include <string>

namespace cs::http {

struct Status {
  Status(uint32_t code, std::string name)
      : code(code), name(name) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const Status& status) {
    return os << status.code << " " << status.name;
  }

  uint32_t code;
  std::string name;
};

static const Status HTTP_200_OK(200, "OK");

}  // namespace cs::http

#endif  // CS_HTTP_STATUS_HH