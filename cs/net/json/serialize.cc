#include "cs/net/json/serialize.hh"

#include <iostream>

#include "cs/net/json/object.hh"

namespace cs::net::json {

std::ostream& operator<<(std::ostream& os,
                         const Object* object) {
  return SearializeObject(os, object);
}

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