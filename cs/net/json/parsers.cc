#include "cs/net/json/parsers.hh"

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
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

ResultOr<float> ParseFloat(std::string str, uint* cursor) {
  if (str.empty()) {
    return Error("Input str is empty.");
  }

  if (cursor == nullptr) {
    return Error("Cursor pointer is null.");
  }

  uint i = 0;
  bool is_negative = false;
  bool has_exponent = false;
  bool negative_exponent = false;
  float decimal_multiplier = 0.1f;
  uint exponent_value = 0;
  float result = 0.0f;

  // Handle leading whitespace.
  while (InBounds(str, i) && std::isspace(str[i])) {
    i++;
  }

  // Handle sign.
  if (InBounds(str, i) &&
      (str[i] == '-' || str[i] == '+')) {
    is_negative = (str[i] == '-');
    i++;
  }

  // Parse digits and build the integer part.
  while (InBounds(str, i) && std::isdigit(str[i])) {
    float digit = static_cast<float>(str[i] - '0');
    result = result * 10.0f + digit;
    i++;
  }

  // Parse decimal point and digits for the fractional part.
  if (InBounds(str, i) && str[i] == '.') {
    i++;
    while (InBounds(str, i) && std::isdigit(str[i])) {
      float digit = static_cast<float>(str[i] - '0');
      result += digit * decimal_multiplier;
      decimal_multiplier *= 0.1f;
      i++;
    }
  }

  // Parse exponent and digits for the exponent part.
  if (InBounds(str, i) &&
      (str[i] == 'e' || str[i] == 'E')) {
    has_exponent = true;
    i++;

    if (InBounds(str, i) &&
        (str[i] == '-' || str[i] == '+')) {
      negative_exponent = (str[i] == '-');
      i++;
    }

    while (InBounds(str, i) && std::isdigit(str[i])) {
      exponent_value = exponent_value * 10 + (str[i] - '0');
      i++;
    }
  }

  // Apply sign and exponent.
  result = (is_negative ? -result : result);
  if (has_exponent) {
    if (negative_exponent) {
      exponent_value *= -1;
    }
    float exponentiated = std::pow(10.0f, exponent_value);
    result *= exponentiated;
  }

  // Update cursor position.
  *cursor = i;

  return result;
}

ResultOr<std::string*> ParseString(std::string str,
                                   uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error("Cursor out of bounds.");
  }

  if (str[*cursor] != '"') {
    return Error("Did not find leading '\"' in string.");
  }
  *cursor += 1;

  std::stringstream ss;
  while (InBounds(str, *cursor)) {
    if (str[*cursor] == '"') {
      break;
    }
    if (str[*cursor] == '\\') {
      *cursor += 1;
    }
    ss << str[*cursor];
    *cursor += 1;
  }
  if (str[*cursor] != '"') {
    return Error("Didn't find ending '\"' character.");
  }
  return new std::string(ss.str());
}

}  // namespace cs::net::json
