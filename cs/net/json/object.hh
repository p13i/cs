#ifndef CS_NET_JSON_OBJECT_HH
#define CS_NET_JSON_OBJECT_HH

#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "cs/net/json/parsers.hh"
#include "cs/result/result.hh"
#include "cs/sanity/ensure.hh"
#include "cs/string/format.h"

using ::cs::net::json::ParseBoolean;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

namespace cs::net::json {

enum class Type {
  UNSET = 0,
  BOOLEAN = 1,
  NUMBER = 2,
  STRING = 3,
  ARRAY = 4,
  OBJECT = 5,
};

/**
 * There are five fundamental types to JSON. They are
 * boolean, float, string, array, and the kev-value mapping.
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
  Object(std::vector<Object> value)
      : _type(Type::ARRAY), _array_value(value) {}
  Object(std::map<std::string, Object> value)
      : _type(Type::OBJECT), _map_value(value) {}

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

  std::vector<Object> as_array() const {
    ENSURE(_type == Type::ARRAY);
    return _array_value;
  }

  std::map<std::string, Object> as_map() const {
    ENSURE(_type == Type::OBJECT);
    return _map_value;
  }

  friend bool operator==(Object a, Object b) {
    // HACK
    return a.to_string() == b.to_string();
  }

  friend Result operator>>(std::string str,
                           Object* object) {
    unsigned int cursor = 0;
    [[maybe_unused]] std::map<std::string, Object>
        map_value;
    [[maybe_unused]] std::vector<Object> array_value;
    [[maybe_unused]] float float_value;
    [[maybe_unused]] std::string string_value;
    [[maybe_unused]] bool bool_value;
    while (cursor < str.length()) {
      char c = str.at(cursor);
      if (c == '{') {
        // Parse map
      } else if (c == '[') {
        // Parse array
      } else if (c == '+' || c == '-' || c == '.' ||
                 ('0' <= c && c <= '9')) {
        // Parse float
        ASSIGN_OR_RETURN(object->_number_value,
                         ParseFloat(str, &cursor));
        object->_type = Type::NUMBER;
      } else if (c == '"') {
        // Parse string
      } else if (c == 't' || c == 'f') {
        // Parse bool
        ASSIGN_OR_RETURN(object->_bool_value,
                         ParseBoolean(str, &cursor));
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

  friend std::ostream& operator<<(std::ostream& os,
                                  const Object& object) {
    if (object._type == Type::BOOLEAN) {
      if (object.as_bool()) {
        os << "true";
      } else {
        os << "false";
      }
    } else if (object._type == Type::NUMBER) {
      os << object.as_number();
    } else if (object._type == Type::STRING) {
      os << '"' << object.as_string() << '"';
    } else if (object._type == Type::ARRAY) {
      os << "[";
      bool first = true;
      for (const auto& elem : object.as_array()) {
        if (!first) {
          os << ",";
        } else {
          first = false;
        }
        os << elem;
      }
      os << "]";
    } else if (object._type == Type::OBJECT) {
      os << "{";
      bool first = true;
      for (const auto& kv : object.as_map()) {
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
  }

  std::string to_string() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

 private:
  Type _type;
  bool _bool_value;
  float _number_value;
  std::string _string_value;
  std::vector<Object> _array_value;
  std::map<std::string, Object> _map_value;
};
}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH