#ifndef CS_NET_HTTP_RESPONSE_HH
#define CS_NET_HTTP_RESPONSE_HH

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/net/http/parsers.hh"
#include "cs/net/http/status.hh"
#include "cs/result/result.hh"
#include "cs/string/format.h"

namespace cs::net::http {

const std::string kContentTypeTextPlain = "text/plain";
const std::string kContentTypeTextHtml = "text/html";
const std::string kContentTypeApplicationJson =
    "application/json";

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

class Response {
 public:
  Response() : Response(HTTP_200_OK) {}
  Response(Status status)
      : Response(status, kContentTypeTextPlain,
                 status.str()) {}
  Response(Status status, std::string content_type,
           std::stringstream ss)
      : Response(status, content_type, ss.str()) {}
  Response(Status status, std::string content_type,
           std::string body)
      : _status(status),
        _content_type(content_type),
        _body(body) {}
  Response(const Result& result) {
    if (result.ok()) {
      _status = HTTP_200_OK;
    } else {
      _status = HTTP_400_BAD_REQUEST;
    }
    _content_type = kContentTypeTextPlain;
    std::stringstream ss;
    ss << _status << ": " << result.message();
    _body = ss.str();
  }
  Response(std::string status_str) {
    if (status_str == HTTP_200_OK.str()) {
      _status = HTTP_200_OK;
    } else if (status_str == HTTP_400_BAD_REQUEST.str()) {
      _status = HTTP_400_BAD_REQUEST;
    }
    _content_type = kContentTypeTextPlain;
  }

  Result Parse(std::string str) {
    _str = str;
    uint cursor = 0;
    std::string http_tag = "";
    // Read any leading whitespace
    while (str[cursor] == ' ' || str[cursor] == '\n' ||
           str[cursor] == '\r') {
      OK_OR_RETURN(IncrementCursor(str, &cursor));
    }
    if (cursor == str.size()) {
      _status = HTTP_500_INTERNAL_SERVER_ERROR;
      return Error(
          "Unable to finding anything but whitespace while "
          "parsing str=`" +
          str + "`");
    }
    // Read HTTP/1.1 tag
    OK_OR_RETURN(ReadWord(str, &cursor, &http_tag, " \n"));
    if (http_tag != "HTTP/1.1") {
      return Error(
          "Didn't find HTTP/1.1 tag at beginning of first "
          "line but found:`" +
          http_tag + "` in `" + str + "`");
    }
    OK_OR_RETURN(IncrementCursor(str, &cursor));
    // Read HTTP status code and message ("400 BAD REQUEST")
    std::string status_str;
    OK_OR_RETURN(ReadWord(str, &cursor, &status_str, " "));
    if (status_str == "400") {
      _status = cs::net::http::HTTP_400_BAD_REQUEST;
    } else if (status_str == "200") {
      _status = cs::net::http::HTTP_200_OK;
    } else if (status_str == "400") {
      _status =
          cs::net::http::HTTP_500_INTERNAL_SERVER_ERROR;
    }
    OK_OR_RETURN(ReadThroughNewline(str, &cursor));

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

  friend std::ostream& operator<<(
      std::ostream& os, const Response& response) {
    return os << "HTTP/1.1 " << response._status << "\r\n"
              << "Content-Type: " << response._content_type
              << "\r\n"
              << "Content-Length: " << response._body.size()
              << "\r\n\r\n"
              << response._body;
  }

  std::string body() { return _body; }

  Status status() { return _status; }

  std::string to_string() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

 private:
  cs::net::http::Status _status;
  std::string _content_type;
  std::string _body;
  std::string _str;
  std::map<std::string, std::string> _headers;
};

}  // namespace cs::net::http

#endif  // CS_NET_HTTP_RESPONSE_HH