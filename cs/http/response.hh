#ifndef CS_HTTP_RESPONSE_HH
#define CS_HTTP_RESPONSE_HH

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/sanity/ensure.hh"

namespace cs::http {

class Response {
 public:
  Response() : Response(200, "") {}
  Response(uint32_t status, std::string body)
      : _status(status), _body(body) {
    ENSURE(status == 200);
  }

  friend std::ostream& operator<<(
      std::ostream& os, const Response& response) {
    return os << "Response(status=" << response._status
              << ", body=" << response._body << ")";
  }

  std::string body() { return _body; }

  std::string to_string() {
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK" << std::endl
       << "Content-Type: text/html" << std::endl
       << "Content-Length: " << _body.size() << std::endl
       << std::endl
       << _body;
    return ss.str();
  }

 private:
  uint32_t _status;
  std::string _body;
};

}  // namespace cs::http

#endif  // CS_HTTP_RESPONSE_HH