#include "cs/net/http/request.hh"

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/net/http/parsers.hh"
#include "cs/result/result.hh"

namespace cs::net::http {

namespace {
using ::cs::net::http::parsers::AtEndOfLine;
using ::cs::net::http::parsers::IncrementCursor;
using ::cs::net::http::parsers::ParsePath;
using ::cs::net::http::parsers::ReadThroughNewline;
using ::cs::net::http::parsers::ReadWord;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
}  // namespace

Result Request::Parse(std::string str) {
  _str = str;
  uint cursor = 0;
  // Read HTTP method
  OK_OR_RETURN(ReadWord(str, &cursor, &_method, " "));
  OK_OR_RETURN(IncrementCursor(str, &cursor));
  // Read HTTP path
  OK_OR_RETURN(ReadWord(str, &cursor, &_path, " "));
  OK_OR_RETURN(ParsePath(_path, &_path, &_query_params));
  OK_OR_RETURN(IncrementCursor(str, &cursor));
  // Read HTTP/1.1 tag
  std::string http_tag = "";
  OK_OR_RETURN(ReadWord(str, &cursor, &http_tag, "\n"));
  if (http_tag != "HTTP/1.1") {
    return Error(
        "Didn't find HTTP/1.1 tag at end of first line.");
  }
  OK_OR_RETURN(IncrementCursor(str, &cursor));
  // Read headers
  bool reading_headers = true;
  while (reading_headers) {
    std::string name;
    OK_OR_RETURN(ReadWord(str, &cursor, &name, ":"));
    OK_OR_RETURN(IncrementCursor(str, &cursor));
    if (str.at(cursor) != ' ') {
      return Error("Expected space after colon.");
    }
    OK_OR_RETURN(IncrementCursor(str, &cursor));
    std::string value;
    OK_OR_RETURN(ReadWord(str, &cursor, &value, "\n"));
    OK_OR_RETURN(ReadThroughNewline(str, &cursor));
    _headers[name] = value;
    if (AtEndOfLine(str, cursor).ok()) {
      reading_headers = false;
    }
  }
  OK_OR_RETURN(ReadThroughNewline(str, &cursor));
  // Read body
  _body = str.substr(cursor, str.length() - cursor);
  return Ok();
}
}  // namespace cs::net::http