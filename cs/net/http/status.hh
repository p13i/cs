#ifndef CS_NET_HTTP_STATUS_HH
#define CS_NET_HTTP_STATUS_HH

#include <stdint.h>

#include <map>
#include <sstream>
#include <string>

namespace cs::net::http {

struct Status {
  Status() {}
  Status(uint32_t code, std::string name)
      : code(code), name(name) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const Status& status) {
    return os << status.code << " " << status.name;
  }

  std::string str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  };

  uint32_t code;
  std::string name;
};

static const Status HTTP_200_OK(200, "OK");
static const Status HTTP_400_BAD_REQUEST(400,
                                         "BAD REQUEST");
static const Status HTTP_404_NOT_FOUND(404, "NOT FOUND");

}  // namespace cs::net::http

#endif  // CS_NET_HTTP_STATUS_HH