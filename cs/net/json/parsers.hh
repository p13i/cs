#ifndef CS_NET_JSON_PARSERS_HH
#define CS_NET_JSON_PARSERS_HH

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "cs/net/json/object.hh"
#include "cs/result/result.hh"
#include "cs/sanity/ensure.hh"
#include "cs/string/format.h"

namespace cs::net::json::parsers {

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
using ::cs::string::format;
}  // namespace

Result operator>>(std::string str, Object* object);

ResultOr<Object*> ParseObject(std::string str,
                              uint* cursor);

ResultOr<std::map<std::string, Object*>> ParseMap(
    std::string str, uint* cursor);

ResultOr<std::string> ParseString(std::string str,
                                  uint* cursor);

ResultOr<bool> ParseBoolean(std::string str, uint* cursor);

ResultOr<float> ParseFloat(std::string str, uint* cursor);

ResultOr<std::vector<Object*>> ParseArray(std::string str,
                                          uint* cursor);

}  // namespace cs::net::json::parsers

#endif  // CS_NET_JSON_PARSERS_HH