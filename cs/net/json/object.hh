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
                              const Object* object);

enum class Type : uint {
  kUnset = 0,
  kBoolean = 1,
  kFloat = 2,
  kString = 3,
  kArray = 4,
  kMap = 5,
};

/**
 * There are five fundamental types to JSON: boolean, float,
 * string, array, and the key-value mapping.
 */
class Object {
 public:
  typedef std::map<std::string, Object*> KVMap;
  Object() : _type(Type::kUnset) {}
  Object(bool value)
      : _type(Type::kBoolean), _bool_value(value) {}
  Object(float number)
      : _type(Type::kFloat), _number_value(number) {}
  Object(std::string value)
      : _type(Type::kString), _string_value(value) {}
  Object(std::vector<float> value) : _type(Type::kArray) {
    for (auto number : value) {
      _array_value.push_back(new Object(number));
    }
  }
  Object(std::vector<Object*> value)
      : _type(Type::kArray), _array_value(value) {}
  Object(KVMap value)
      : _type(Type::kMap), _map_value(value) {}

  template <typename T>
  T as(T instance);

  float as(float instance) const {
    if (_type != Type::kFloat) {
      return NAN;
    }
    return _number_value;
  }

  bool as(bool instance) const {
    if (_type != Type::kBoolean) {
      return false;
    }
    return _bool_value;
  }

  std::string as(std::string instance) const {
    if (_type != Type::kString) {
      return "";
    }
    return _string_value;
  }

  std::string as(const char* instance) const {
    if (_type != Type::kString) {
      return "";
    }
    return _string_value;
  }

  KVMap as(KVMap instance) const {
    if (_type != Type::kMap) {
      return {};
    }
    return _map_value;
  }

  ~Object() {
    if (_type == Type::kArray) {
      for (auto* obj : _array_value) {
        delete obj;
      }
    } else if (_type == Type::kMap) {
      for (auto& [key, obj] : _map_value) {
        delete obj;
      }
    }
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

  std::vector<Object*> as_array() const {
    return _array_value;
  }

  std::map<std::string, Object*> as_map() const {
    return _map_value;
  }

  bool has_key(const std::string& key) {
    if (_type != Type::kMap) {
      return false;
    }
    return _map_value.find(key) != _map_value.end();
  }

  template <typename T>
  ResultOr<T> get(const std::string& key,
                  T instance) const {
    if (_type != Type::kMap) {
      return Error("Object is not a map.");
    }
    if (_map_value.find(key) == _map_value.end()) {
      return Error(format("Key not found: %s", key));
    }
    return _map_value.at(key)->as(instance);
  }

  ResultOr<Object*> get(const std::string& key) const {
    if (_type != Type::kMap) {
      return Error("Object is not a map.");
    }
    if (_map_value.find(key) == _map_value.end()) {
      return Error(format("Key not found: %s", key));
    }
    return _map_value.at(key);
  }

  ResultOr<Object*> operator[](const std::string& key) {
    return get(key);
  }

  Type _type;

  bool _bool_value;
  float _number_value;
  std::string _string_value;
  std::vector<Object*> _array_value;
  std::map<std::string, Object*> _map_value;
};

template <typename T>
inline T Object::as(T) {
  return T();
}

}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH