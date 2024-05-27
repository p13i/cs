#include <stdint.h>

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "cs/result/result.hh"

namespace cs::net::http::parsers {

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
}  // namespace

Result AtEndOfLine(std::string str, uint cursor);

Result IncrementCursor(std::string str, uint* cursor);

#define STRING_CONTAINS(str, ch) \
  (str.find(ch) != std::string::npos)

Result ReadWord(std::string str, uint* cursor,
                std::string* token,
                std::string ending_tokens);

Result ReadThroughNewline(std::string str, uint* cursor);

Result ParsePath(
    std::string original_path, std::string* just_path,
    std::map<std::string, std::string>* query_params);

}  // namespace cs::net::http::parsers
