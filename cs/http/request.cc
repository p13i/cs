#include "cs/http/request.hh"

#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/result/result.hh"
#include "cs/sanity/ensure.hh"

namespace cs::http {

namespace {

using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

Result AtEndOfLine(std::string str, uint cursor) {
  if (cursor >= str.length()) {
    return Error("cursor exceeded string length");
  }
  if (str.at(cursor) == '\n') {
    return Ok();
  }
  if ((cursor + 1) < str.length() &&
      str.at(cursor) == '\r' &&
      str.at(cursor + 1) == '\n') {
    return Ok();
  };
  return Error("failed to find end of line");
}

Result IncrementCursor(std::string str, uint* cursor) {
  *cursor = *cursor + 1;
  if (*cursor >= str.length()) {
    std::stringstream ss;
    ss << "cursor exceeded string length. ";
    ss << "*cursor=" << *cursor << ", ";
    ss << "str.length()=" << str.length() << ", ";
    ss << "str=" << str;
    return Error(ss.str());
  }
  return Ok();
}

#define STRING_CONTAINS(str, ch) \
  (str.find(ch) != std::string::npos)

Result ReadWord(std::string str, uint* cursor,
                std::string* token,
                std::string ending_tokens) {
  std::stringstream ss;
  char c;
  while (*cursor < str.length() &&
         !STRING_CONTAINS(ending_tokens, str.at(*cursor))) {
    c = str.at(*cursor);
    if (c != '\r') {
      ss << c;
    }
    Result increment_res = IncrementCursor(str, cursor);
    if (STRING_CONTAINS(ending_tokens, '\n') &&
        *cursor >= str.length()) {
      break;
    } else if (!increment_res.ok()) {
      return increment_res;
    }
  }
  if (*cursor == str.length() &&
      !STRING_CONTAINS(ending_tokens, '\n')) {
    return Error("cursor exceeded string length");
  }
  *token = ss.str();
  return Ok();
}

Result ReadThroughNewline(std::string str, uint* cursor) {
  std::stringstream ss;
  bool found_newline = false;
  while (*cursor < str.length() && !found_newline) {
    if (str.at(*cursor) == '\n') {
      found_newline = true;
    }
    IncrementCursor(str, cursor);
  }
  if (!found_newline) {
    return Error("failed to find newline");
  }
  return Ok();
};

Result ParsePath(std::string original_path,
                 std::string* just_path,
                 QueryParams* query_params) {
  // Find just the path before the query params
  uint cursor = 0;
  Result read_to_qmark =
      ReadWord(original_path, &cursor, just_path, "?");
  std::cout << "original_path=" << original_path
            << ", cursor=" << cursor
            << ", just_path=" << *just_path << std::endl;
  if (cursor >= original_path.size()) {
    return Ok();
  } else if (!read_to_qmark.ok()) {
    return read_to_qmark;
  }
  // Parse query params string
  ENSURE(original_path.at(cursor) == '?');
  ENSURE_OK(IncrementCursor(original_path, &cursor));
  while (cursor < original_path.size()) {
    std::string name;
    ENSURE_OK(ReadWord(original_path, &cursor, &name, "="));
    ENSURE_OK(IncrementCursor(original_path, &cursor));
    std::string value;
    ENSURE_OK(
        ReadWord(original_path, &cursor, &value, "&\n"));
    query_params->insert({name, value});
    if (cursor >= original_path.size()) {
      break;
    }
    ENSURE(original_path.at(cursor) == '&');
    ENSURE_OK(IncrementCursor(original_path, &cursor));
  }
  return Ok();
}

}  // namespace

Result Request::Parse(std::string str) {
  uint cursor = 0;
  // Read HTTP method
  ENSURE_OK(ReadWord(str, &cursor, &_method, " "));
  ENSURE_OK(IncrementCursor(str, &cursor));
  // Read HTTP path
  ENSURE_OK(ReadWord(str, &cursor, &_path, " "));
  ENSURE_OK(ParsePath(_path, &_path, &_query_params));
  ENSURE_OK(IncrementCursor(str, &cursor));
  // Read HTTP/1.1 tag
  std::string http_tag = "";
  ENSURE_OK(ReadWord(str, &cursor, &http_tag, "\n"));
  if (http_tag != "HTTP/1.1") {
    return Error(
        "Didn't find HTTP/1.1 tag at end of first line.");
  }
  ENSURE_OK(IncrementCursor(str, &cursor));
  // Read headers
  bool reading_headers = true;
  while (reading_headers) {
    std::string name;
    ENSURE_OK(ReadWord(str, &cursor, &name, ":"));
    ENSURE_OK(IncrementCursor(str, &cursor));
    std::string value;
    ENSURE_OK(ReadWord(str, &cursor, &value, "\n"));
    ENSURE_OK(ReadThroughNewline(str, &cursor));
    _headers[name] = value;
    if (AtEndOfLine(str, cursor).ok()) {
      reading_headers = false;
    }
  }
  ENSURE_OK(ReadThroughNewline(str, &cursor));
  // Read body
  _body = str.substr(cursor, str.length() - cursor);
  return Ok();
}
}  // namespace cs::http