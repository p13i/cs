#ifndef CS_HTTP_REQUEST_HH
#define CS_HTTP_REQUEST_HH

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/sanity/ensure.hh"

namespace cs::http {

class Request {
 public:
  Request(std::string str) { ENSURE(Parse(str)); };

  friend std::ostream& operator<<(std::ostream& os,
                                  const Request& request) {
    os << "Request(method=" << request._method
       << ", path=" << request._path << ", headers=";
    // Print headers
    for (auto it = request._headers.begin();
         it != request._headers.end(); it++) {
      os << "<name=" << it->first
         << ", value=" << it->second << ">, ";
    }
    os << "body=" << request._body << ")";
    return os;
  }

  bool Parse(std::string str);

 private:
  std::string _method;
  std::string _path;
  std::map<std::string, std::string> _headers;
  std::string _body;
};

}  // namespace cs::http

#endif  // CS_HTTP_REQUEST_HH