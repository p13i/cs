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

Result MaybeConsumeWhitespace(std::string str,
                              uint* cursor);

// JSON serialization algorithm.
std::ostream& SearializeObject(std::ostream& os,
                               const Object* object);

enum class Type : uint {
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
      : _type(Type::STRING), _string_value(value) {}
  Object(std::vector<Object*> value)
      : _type(Type::ARRAY), _array_value(value) {}
  Object(std::map<std::string, Object*> value)
      : _type(Type::MAP), _map_value(value) {}

  Type type() { return _type; }

  bool as_bool() const {
    ENSURE(_type == Type::BOOLEAN);
    return _bool_value;
  }

  float as_number() const {
    ENSURE(_type == Type::NUMBER);
    return _number_value;
  }

  std::string as_string() const {
    ENSURE(_type == Type::STRING);
    return _string_value;
  }

  std::vector<Object*> as_array() const {
    ENSURE(_type == Type::ARRAY);
    return _array_value;
  }

  std::map<std::string, Object*> as_map() const {
    ENSURE(_type == Type::MAP);
    return _map_value;
  }

  friend bool operator==(Object a, Object b) {
    // HACK
    return a.to_string() == b.to_string();
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
  std::string _string_value;
  std::vector<Object*> _array_value;
  std::map<std::string, Object*> _map_value;
};

std::ostream& SearializeObject(std::ostream& os,
                               const Object* object) {
  if (object->_type == Type::BOOLEAN) {
    if (object->as_bool()) {
      os << "true";
    } else {
      os << "false";
    }
  } else if (object->_type == Type::NUMBER) {
    os << object->as_number();
  } else if (object->_type == Type::STRING) {
    os << '"' << object->as_string() << '"';
  } else if (object->_type == Type::ARRAY) {
    os << "[";
    bool first = true;
    for (const auto& elem : object->as_array()) {
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
    for (const auto& kv : object->as_map()) {
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

}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH