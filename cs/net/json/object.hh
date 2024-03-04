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

  ~Object() {
    if (_type == Type::ARRAY) {
      for (auto* obj : _array_value) {
        delete obj;
      }
    } else if (_type == Type::MAP) {
      for (auto& [key, obj] : _map_value) {
        delete obj;
      }
    }
  }

  Type type() { return _type; }

  bool as_bool() const { return _bool_value; }

  float as_number() const { return _number_value; }

  std::string as_string() const { return _string_value; }

  std::vector<Object*> as_array() const {
    return _array_value;
  }

  std::map<std::string, Object*> as_map() const {
    return _map_value;
  }

  Type _type;

  bool _bool_value;
  float _number_value;
  std::string _string_value;
  std::vector<Object*> _array_value;
  std::map<std::string, Object*> _map_value;
};

}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH