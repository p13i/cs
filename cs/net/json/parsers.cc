#include "cs/net/json/parsers.hh"

#include <map>
#include <string>
#include <vector>

#include "cs/result/result.hh"
#include "cs/string/format.h"

namespace cs::net::json {

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
using ::cs::string::format;

bool InBounds(std::string str, uint cursor) {
  return cursor < str.size();
}

bool TryConsumeString(std::string str, std::string target,
                      uint* cursor) {
  uint str_cursor = *cursor;
  uint target_cursor = 0;

  while (InBounds(target, target_cursor)) {
    if (!InBounds(str, str_cursor)) {
      return false;
    }
    if (str[str_cursor] != target[target_cursor]) {
      return false;
    }
    str_cursor++;
    target_cursor++;
  }
  *cursor = str_cursor;
  return true;
}
}  // namespace

ResultOr<bool> ParseBoolean(std::string str, uint* cursor) {
  const std::string TRUE = "true";
  const std::string FALSE = "false";

  if (TryConsumeString(str, "true", cursor)) {
    return true;
  }

  if (TryConsumeString(str, "false", cursor)) {
    return false;
  }

  return Error(cs::string::format(
      "Didn't find boolean at str[*cursor:]=%s",
             str.substr(*cursor)));
}

}  // namespace cs::net::json
