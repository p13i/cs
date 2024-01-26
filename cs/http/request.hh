#include <stdint.h>

#include <ostream>
#include <sstream>
#include <string>

#include "cs/sanity/ensure.hh"

namespace cs::http {

namespace {

bool ReadToken(std::string bytes, uint* cursor,
               std::string* token,
               char ending_token = ' ') {
  std::stringstream ss;
  while (*cursor < bytes.length() &&
         bytes.at(*cursor) != ending_token) {
    char c = bytes.at(*cursor);
    if (c != '\r') {
      ss << c;
    }
    *cursor = *cursor + 1;
  }
  if (*cursor == bytes.length()) {
    return false;
  }
  *token = ss.str();
  return true;
};

}  // namespace

class Request {
 public:
  Request(std::string bytes) {
    uint cursor = 0;
    ENSURE(ReadToken(bytes, &cursor, &_method, ' '));
    cursor = cursor + 1;
    ENSURE(ReadToken(bytes, &cursor, &_path, ' '));
    cursor = cursor + 1;
    std::string http_tag = "";
    ENSURE(ReadToken(bytes, &cursor, &http_tag, '\n'));
    ENSURE(http_tag == "HTTP/1.1");
  };

  friend std::ostream& operator<<(std::ostream& os,
                                  const Request& request) {
    return os << "Request(method=" << request._method
              << ")";
  }

 private:
  std::string _method;
  std::string _path;
  std::string _body;
};

class Response {
 public:
  std::string to_string() { return ""; }

 private:
  uint8_t _status;
  std::string _html;
};

}  // namespace cs::http