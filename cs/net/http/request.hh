#ifndef CS_NET_HTTP_REQUEST_HH
#define CS_NET_HTTP_REQUEST_HH

#include <stdint.h>

#include <map>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/result/result.hh"

namespace cs::net::http {

using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;

typedef std::map<std::string, std::string> QueryParams;

namespace {
std::ostream& operator<<(std::ostream& os,
                         const QueryParams& map) {
  os << "{";
  bool first = true;
  for (auto it = map.begin(); it != map.end(); it++) {
    if (!first) {
      os << ", ";
    } else {
      first = false;
    }
    os << "<key=" << it->first << ", value=" << it->second
       << ">";
  }
  os << "}";
  return os;
}
}  // namespace

class Request {
 public:
  Request(){};

  friend std::ostream& operator<<(std::ostream& os,
                                  const Request& request) {
    return os << request._str;
  }

  friend Result operator>>(char buffer[],
                           Request& request) {
    return request.Parse(std::string(buffer));
  }

  Result Parse(std::string str);

  std::string path() { return _path; }

  std::string method() { return _method; }

  std::optional<std::string> get_query_param(
      std::string name) {
    auto found = _query_params.find(name);
    if (found == _query_params.end()) {
      return std::nullopt;
    }
    return found->second;
  }

  ResultOr<std::string> GetQueryParam(std::string name) {
    auto found = _query_params.find(name);
    if (found == _query_params.end()) {
      return Error("no query param with name " + name +
                   " found.");
    }
    return found->second;
  }

  std::string body() { return _body; }

  std::map<std::string, std::string> headers() {
    return _headers;
  }

 private:
  std::string _method;
  std::string _path;
  QueryParams _query_params;
  std::map<std::string, std::string> _headers;
  std::string _body;
  std::string _str;
};

}  // namespace cs::net::http

#endif  // CS_NET_HTTP_REQUEST_HH