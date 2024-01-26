#ifndef CS_HTTP_REQUEST_HH
#define CS_HTTP_REQUEST_HH

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/sanity/ensure.hh"

namespace cs::http {

namespace {

bool AtEndOfLine(std::string str, uint cursor) {
  if (cursor >= str.length()) {
    return false;
  }
  if (str.at(cursor) == '\n') {
    return true;
  }
  if ((cursor + 1) < str.length()) {
    return str.at(cursor) == '\r' &&
           str.at(cursor + 1) == '\n';
  }
  return false;
}

bool IncrementCursor(std::string str, uint* cursor) {
  *cursor = *cursor + 1;
  return *cursor < str.length();
}

bool ReadToken(std::string str, uint* cursor,
               std::string* token,
               char ending_token = ' ') {
  std::stringstream ss;
  while (*cursor < str.length() &&
         str.at(*cursor) != ending_token) {
    char c = str.at(*cursor);
    if (c != '\r') {
      ss << c;
    }
    IncrementCursor(str, cursor);
  }
  if (*cursor == str.length()) {
    return false;
  }
  *token = ss.str();
  return true;
};

bool ReadThroughNewline(std::string str, uint* cursor) {
  std::stringstream ss;
  bool found_newline = false;
  while (*cursor < str.length() && !found_newline) {
    if (str.at(*cursor) == '\n') {
      found_newline = true;
    }
    IncrementCursor(str, cursor);
  }
  return found_newline;
};

}  // namespace

class Request {
 public:
  Request(std::string str) {
    uint cursor = 0;
    // Read HTTP method
    ENSURE(ReadToken(str, &cursor, &_method));
    IncrementCursor(str, &cursor);
    // Read HTTP path
    ENSURE(ReadToken(str, &cursor, &_path));
    IncrementCursor(str, &cursor);
    // Read HTTP/1.1 tag
    std::string http_tag = "";
    ENSURE(ReadToken(str, &cursor, &http_tag, '\n'));
    ENSURE(http_tag == "HTTP/1.1");
    IncrementCursor(str, &cursor);
    // Read headers
    bool reading_headers = true;
    while (reading_headers) {
      std::string name;
      ENSURE(ReadToken(str, &cursor, &name, ':'));
      IncrementCursor(str, &cursor);
      std::string value;
      ENSURE(ReadToken(str, &cursor, &value, '\n'));
      ENSURE(ReadThroughNewline(str, &cursor));
      _headers[name] = value;
      if (AtEndOfLine(str, cursor)) {
        reading_headers = false;
      }
    }
    ENSURE(ReadThroughNewline(str, &cursor));
    // Read body
    std::stringstream ss;
    while (cursor < str.length()) {
      ss << str.at(cursor);
      IncrementCursor(str, &cursor);
    }
    _body = ss.str();
  };

  friend std::ostream& operator<<(std::ostream& os,
                                  const Request& request) {
    os << "Request(method=" << request._method
       << ", path=" << request._path << ", headers=";

    for (auto it = request._headers.begin();
         it != request._headers.end(); it++) {
      os << "<name=" << it->first
         << ", value=" << it->second << ">,";
    }
    os << ")";
    return os;
  }

 private:
  std::string _method;
  std::string _path;
  std::map<std::string, std::string> _headers;
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

#endif  // CS_HTTP_REQUEST_HH