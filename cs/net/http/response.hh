#ifndef CS_NET_HTTP_RESPONSE_HH
#define CS_NET_HTTP_RESPONSE_HH

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/net/http/status.hh"
#include "cs/sanity/ensure.hh"

namespace cs::net::http {

const std::string kContentTypeTextPlain = "text/plain";
const std::string kContentTypeTextHtml = "text/html";
const std::string kContentTypeTextJson = "text/json";

class Response {
 public:
  Response() : Response(HTTP_200_OK) {}
  Response(Status status)
      : Response(status, kContentTypeTextPlain,
                 status.str()) {}
  Response(Status status, std::string content_type,
           std::string body)
      : _status(status),
        _content_type(content_type),
        _body(body) {}

  friend std::ostream& operator<<(
      std::ostream& os, const Response& response) {
    return os << "Response(status=" << response._status
              << ", body=" << response._body.substr(0, 128)
              << ")";
  }

  std::string body() { return _body; }

  Status status() { return _status; }

  std::string to_string() {
    std::stringstream ss;
    ss << "HTTP/1.1 " << _status << std::endl
       << "Content-Type: " << _content_type << std::endl
       << "Content-Length: " << _body.size() << std::endl
       << std::endl
       << _body << std::endl;
    return ss.str();
  }

 private:
  Status _status;
  std::string _content_type;
  std::string _body;
};

}  // namespace cs::net::http

#endif  // CS_NET_HTTP_RESPONSE_HH