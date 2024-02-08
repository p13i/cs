#ifndef CS_NET_JSON_PARSERS_HH
#define CS_NET_JSON_PARSERS_HH

#include <map>
#include <vector>

#include "cs/result/result.hh"

namespace cs::net::json {

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

ResultOr<bool> ParseBoolean(std::string str,
                            unsigned int* cursor);

}  // namespace cs::net::json

#endif  // CS_NET_JSON_PARSERS_HH