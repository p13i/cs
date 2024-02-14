#include "cs/net/json/parsers.hh"

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

Result operator>>(std::string str, Object* object) {
  uint cursor = 0;
  ASSIGN_OR_RETURN(*object, ParseObject(str, &cursor));
  return Ok();
}

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
using ::cs::string::format;

bool InBounds(std::string str, uint cursor) {
  return cursor < str.size();
}

Result IncrementCursor(std::string str, uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error(cs::string::format(
        "Cursor out of bounds: str=%s, cursor=%d", str,
        *cursor));
  }
  *cursor += 1;
  return Ok();
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
    str_cursor += 1;
    target_cursor += 1;
  }
  *cursor = str_cursor;
  return true;
}

}  // namespace

ResultOr<bool> ParseBoolean(std::string str, uint* cursor) {
  if (TryConsumeString(str, "true", cursor)) {
    std::cerr << "Found true at cursor=" << *cursor
              << std::endl;
    return true;
  }

  if (TryConsumeString(str, "false", cursor)) {
    std::cerr << "Found true at cursor=" << *cursor
              << std::endl;
    return false;
  }

  return Error(cs::string::format(
      "Didn't find boolean at str[*cursor:]=%s",
      str.substr(*cursor)));
}

ResultOr<float> ParseFloat(std::string str, uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error(cs::string::format(
        "Cursor out of bounds: str=%s, cursor=%d", str,
        *cursor));
  }

  bool is_negative = false;
  bool has_exponent = false;
  bool negative_exponent = false;
  float decimal_multiplier = 0.1f;
  uint exponent_value = 0;
  float result = 0.0f;

  // Handle sign.
  if (InBounds(str, *cursor) &&
      (str[*cursor] == '-' || str[*cursor] == '+')) {
    is_negative = (str[*cursor] == '-');
    *cursor += 1;
  }

  // Parse digits and build the integer part.
  while (InBounds(str, *cursor) && std::isdigit(str[*cursor])) {
    float digit = static_cast<float>(str[*cursor] - '0');
    result = result * 10.0f + digit;
    *cursor += 1;
  }

  // Parse decimal point and digits for the fractional
  // part.
  if (InBounds(str, *cursor) && str[*cursor] == '.') {
    *cursor += 1;
    while (InBounds(str, *cursor) &&
           std::isdigit(str[*cursor])) {
      float digit = static_cast<float>(str[*cursor] - '0');
      result += digit * decimal_multiplier;
      decimal_multiplier *= 0.1f;
      *cursor += 1;
    }
  }

  // Parse exponent and digits for the exponent part.
  if (InBounds(str, *cursor) &&
      (str[*cursor] == 'e' || str[*cursor] == 'E')) {
    has_exponent = true;
    *cursor += 1;

    if (InBounds(str, *cursor) &&
        (str[*cursor] == '-' || str[*cursor] == '+')) {
      negative_exponent = (str[*cursor] == '-');
      *cursor += 1;
    }

    while (InBounds(str, *cursor) &&
           std::isdigit(str[*cursor])) {
      exponent_value =
          exponent_value * 10 + (str[*cursor] - '0');
      *cursor += 1;
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

  return result;
}

ResultOr<std::string> ParseString(std::string str,
                                  uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error(cs::string::format(
        "Cursor out of bounds: str=%s, cursor=%d", str,
        *cursor));
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
    ss << str[*cursor];
    *cursor += 1;
  }
  if (str[*cursor] != '"') {
    return Error("Didn't find ending '\"' character.");
  }
  OK_OR_RETURN(IncrementCursor(str, cursor));
  return std::string(ss.str());
}

ResultOr<std::vector<Object*>> ParseArray(std::string str,
                                          uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error(cs::string::format(
        "Cursor out of bounds: str=%s, cursor=%d", str,
        *cursor));
  }

  if (str[*cursor] != '[') {
    return Error("Did not find leading '[' in array.");
  }
  *cursor += 1;

  std::vector<Object*> array;

  while (InBounds(str, *cursor)) {
    if (str[*cursor] == ']') {
      break;
    }
#if 0
    Object* object;
    ASSIGN_OR_RETURN(object, ParseObject(str, cursor));
#endif
    float value;
    ASSIGN_OR_RETURN(value, ParseFloat(str, cursor));
    Object* object = new Object(value);
    object->_type = Type::NUMBER;
    object->_number_value = value;
    array.push_back(object);
  }

  if (str[*cursor] != ']') {
    return Error("Did not find ending ']' in array.");
  }
  *cursor += 1;

  return array;
}

ResultOr<Object*> ParseObject(std::string str,
                              uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error(cs::string::format(
        "Cursor out of bounds: str=%s, cursor=%d", str,
        *cursor));
  }

  Object* object = new Object();
  while (InBounds(str, *cursor)) {
    char c = str[*cursor];
    if (c == '{') {
      // TODO Parse map
    } else if (c == '[') {
      // TODO Parse array
      std::cout << "Parsing array" << std::endl;
      ASSIGN_OR_RETURN(object->_array_value,
                       ParseArray(str, cursor));
      object->_type = Type::ARRAY;
    } else if (c == '+' || c == '-' || c == '.' ||
               ('0' <= c && c <= '9')) {
      // Parse float
      ASSIGN_OR_RETURN(object->_number_value,
                       ParseFloat(str, cursor));
      object->_type = Type::NUMBER;
    } else if (c == '"') {
      // Parse string
      ASSIGN_OR_RETURN(object->_string_value,
                       ParseString(str, cursor));
      object->_type = Type::STRING;
    } else if (c == 't' || c == 'f') {
      // Parse bool
      ASSIGN_OR_RETURN(object->_bool_value,
                       ParseBoolean(str, cursor));
      object->_type = Type::BOOLEAN;
    } else {
      return Error(cs::string::format(
          "Reached unexpected character ('%c') at "
          "cursor=%d",
          c, cursor));
    }
  }

  return object;
}
}  // namespace cs::net::json::parsers