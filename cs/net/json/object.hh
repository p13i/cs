#ifndef CS_NET_JSON_OBJECT_HH
#define CS_NET_JSON_OBJECT_HH

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "cs/result/result.hh"
#include "cs/sanity/ensure.hh"
#include "cs/string/format.h"

namespace cs::net::json {

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
using ::cs::string::format;
}  // namespace

class Object;

ResultOr<Object*> ParseObject(std::string str,
                              uint* cursor);

ResultOr<std::string*> ParseString(std::string str,
                                   uint* cursor);

ResultOr<bool> ParseBoolean(std::string str, uint* cursor);

ResultOr<float> ParseFloat(std::string str, uint* cursor);

// JSON serialization algorithm.
std::ostream& SearializeObject(std::ostream& os,
                               const Object* object);

enum class Type {
  UNSET = 0,
  BOOLEAN = 1,
  NUMBER = 2,
  STRING = 3,
  ARRAY = 4,
  MAP = 5,
};

/**
 * There are five fundamental types to JSON. They are
 * boolean, float, string, array, and the kev-value
 * mapping.
 */
class Object {
 public:
  Object() : _type(Type::UNSET) {}
  Object(bool value)
      : _type(Type::BOOLEAN), _bool_value(value) {}
  Object(float number)
      : _type(Type::NUMBER), _number_value(number) {}
  Object(std::string value)
      : _type(Type::STRING), _string_value(&value) {}
  Object(std::vector<Object*> value)
      : _type(Type::ARRAY), _array_value(value) {}
  Object(std::map<std::string, Object*> value)
      : _type(Type::MAP), _map_value(value) {}

  Type type() { return _type; }

  ResultOr<bool> as_bool() const {
    if (_type != Type::BOOLEAN) {
      return Error("Object is not a boolean.");
    }
    return _bool_value;
  }

  ResultOr<float> as_number() const {
    ENSURE(_type == Type::NUMBER);
    return _number_value;
  }

  ResultOr<std::string> as_string() const {
    ENSURE(_type == Type::STRING);
    return *_string_value;
  }

  ResultOr<std::vector<Object*>> as_array() const {
    ENSURE(_type == Type::ARRAY);
    return _array_value;
  }

  ResultOr<std::map<std::string, Object*>> as_map() const {
    ENSURE(_type == Type::MAP);
    return _map_value;
  }

  friend bool operator==(Object a, Object b) {
    // HACK
    return a.to_string() == b.to_string();
  }

  friend Result operator>>(std::string str,
                           Object* object) {
    uint cursor = 0;
    ASSIGN_OR_RETURN(*object, ParseObject(str, &cursor));
    return Ok();
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Object* object) {
    return SearializeObject(os, object);
  }

  std::string to_string() {
    std::stringstream ss;
    ss << this;
    return ss.str();
  }

  Type _type;

  bool _bool_value;
  float _number_value;
  std::string* _string_value;
  std::vector<Object*> _array_value;
  std::map<std::string, Object*> _map_value;
};


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
    str_cursor += 1;
    target_cursor += 1;
  }
  *cursor = str_cursor;
  return true;
}

std::ostream& SearializeObject(std::ostream& os,
                               const Object* object) {
  if (object->_type == Type::BOOLEAN) {
    if (object->as_bool().value()) {
      os << "true";
    } else {
      os << "false";
    }
  } else if (object->_type == Type::NUMBER) {
    os << object->as_number().value();
  } else if (object->_type == Type::STRING) {
    os << '"' << object->as_string().value() << '"';
  } else if (object->_type == Type::ARRAY) {
    os << "[";
    bool first = true;
    for (const auto& elem : object->as_array().value()) {
      if (!first) {
        os << ",";
      } else {
        first = false;
      }
      os << elem;
    }
    os << "]";
  } else if (object->_type == Type::MAP) {
    os << "{";
    bool first = true;
    for (const auto& kv : object->as_map().value()) {
      if (!first) {
        os << ",";
      } else {
        first = false;
      }
      os << '"' << kv.first << '"' << ":" << kv.second;
    }
    os << "}";
  }
  return os;
};

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

  // Parse decimal point and digits for the fractional
  // part.
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

Result IncrementCursor(std::string str, uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error("Cursor out of bounds.");
  }
  *cursor += 1;
  return Ok();
}

ResultOr<std::vector<Object*>*> ParseArray(std::string str,
                                           uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error("Cursor out of bounds.");
  }

  if (str[*cursor] != '[') {
    return Error("Did not find leading '[' in array.");
  }
  *cursor += 1;

  std::vector<Object*>* array = new std::vector<Object*>();

  return array;
}

Result MaybeConsumeWhitespace(std::string str,
                              uint* cursor) {
  while (InBounds(str, *cursor) &&
         std::isspace(str[*cursor])) {
    IncrementCursor(str, cursor);
  }
  return Ok();
}

ResultOr<Object*> ParseObject(std::string str,
                              uint* cursor) {
  if (!InBounds(str, *cursor)) {
    return Error("Cursor out of bounds.");
  }

  Object* object = new Object();
  while (InBounds(str, *cursor)) {
    char c = str[*cursor];
    if (c == '{') {
      // TODO Parse map
    } else if (c == '[') {
// TODO Parse array
#if 0
        ASSIGN_OR_RETURN(object->array_value,
                         ParseArray(str, &cursor));
        object->_type = Type::ARRAY;
#endif
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
  return Ok();
}
}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH