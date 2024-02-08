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
}  // namespace

ResultOr<bool> ParseBoolean(std::string str,
                            unsigned int* cursor) {
  const std::string TRUE = "true";
  const std::string FALSE = "false";

  if (str.substr(*cursor, TRUE.size()) == TRUE) {
    *cursor += TRUE.size();
    return true;
  } else if (str.substr(*cursor, FALSE.size()) == FALSE) {
    *cursor += FALSE.size();
    return false;
  }
  return Error(
      format("Didn't find boolean at str[*cursor:]=%s",
             str.substr(*cursor)));
}

}  // namespace cs::net::json
