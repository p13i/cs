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
std::ostream& SerializeObject(std::ostream& os,
                              const Object object);

enum class Type : uint {
  UNSET = 0,
  BOOLEAN = 1,
  FLOAT = 2,
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
  typedef std::map<std::string, Object> KVMap;
  Object() : _type(Type::UNSET) {}
  Object(bool value)
      : _type(Type::BOOLEAN), _bool_value(value) {}
  Object(float number)
      : _type(Type::FLOAT), _number_value(number) {}
  Object(std::string value)
      : _type(Type::STRING), _string_value(value) {}
  Object(std::vector<float> value) : _type(Type::ARRAY) {
    for (auto number : value) {
      _array_value.push_back(Object(number));
    }
  }
  Object(std::vector<Object> value)
      : _type(Type::ARRAY), _array_value(value) {}
  Object(KVMap value)
      : _type(Type::MAP), _map_value(value) {}

  template <typename T>
  T as(T instance);

  float as(float instance) const {
    if (_type != Type::FLOAT) {
      return NAN;
    }
    return _number_value;
  }

  bool as(bool instance) const {
    if (_type != Type::BOOLEAN) {
      return false;
    }
    return _bool_value;
  }

  std::string as(std::string instance) const {
    if (_type != Type::STRING) {
      return "";
    }
    return _string_value;
  }

  std::string as(const char* instance) const {
    if (_type != Type::STRING) {
      return "";
    }
    return _string_value;
  }

  KVMap as(KVMap instance) const {
    if (_type != Type::MAP) {
      return {};
    }
    return _map_value;
  }

  Type type() { return _type; }

  bool as_bool() const { return this->as(bool()); }

  float as_number() const { return this->as(float()); }

  std::string as_string() const {
    return this->as(std::string());
  }

  std::string str() const {
    std::stringstream ss;
    SerializeObject(ss, this);
    return ss.str();
  }

  std::vector<Object> as_array() const {
    return _array_value;
  }

  KVMap as_map() const { return _map_value; }

  bool has_key(const std::string& key) {
    if (_type != Type::MAP) {
      return false;
    }
    return _map_value.find(key) != _map_value.end();
  }

  template <typename T>
  ResultOr<T> get(const std::string& key,
                  T instance) const {
    if (_type != Type::MAP) {
      return Error("Object is not a map.");
    }
    if (_map_value.find(key) == _map_value.end()) {
      return Error(format("Key not found: %s", key));
    }
    return _map_value.at(key).as(instance);
  }

  ResultOr<Object> get(const std::string& key) const {
    if (_type != Type::MAP) {
      return Error("Object is not a map.");
    }
    if (_map_value.find(key) == _map_value.end()) {
      return Error(format("Key not found: %s", key));
    }
    return _map_value.at(key);
  }

  ResultOr<Object> operator[](const std::string& key) {
    return get(key);
  }

  Type _type;

  bool _bool_value;
  float _number_value;
  std::string _string_value;
  std::vector<Object> _array_value;
  KVMap _map_value;
};

template <typename T>
inline T Object::as(T) {
  return T();
}

}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH