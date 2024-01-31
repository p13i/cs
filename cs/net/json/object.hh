#ifndef CS_NET_JSON_OBJECT_HH
#define CS_NET_JSON_OBJECT_HH

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cs/sanity/ensure.hh"

namespace cs::net::json {
enum class Type {
  BOOLEAN,
  NUMBER,
  STRING,
  ARRAY,
  OBJECT,
};
class Object {
 public:
  Object(bool value)
      : _type(Type::BOOLEAN), _bool_value(value) {}
  Object(float number)
      : _type(Type::NUMBER), _number_value(number) {}
  Object(std::string value)
      : _type(Type::STRING), _string_value(value) {}
  Object(std::vector<Object> value)
      : _type(Type::ARRAY), _array_value(value) {}
  Object(std::map<std::string, Object> value)
      : _type(Type::OBJECT), _object_value(value) {}

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

  std::map<std::string, Object> as_object() const {
    ENSURE(_type == Type::OBJECT);
    return _object_value;
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
      for (const auto& kv : object.as_object()) {
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

 private:
  Type _type;
  bool _bool_value;
  float _number_value;
  std::string _string_value;
  std::vector<Object> _array_value;
  std::map<std::string, Object> _object_value;
};
}  // namespace cs::net::json

#endif  // CS_NET_JSON_OBJECT_HH