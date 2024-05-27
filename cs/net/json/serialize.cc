#include "cs/net/json/serialize.hh"

#include <iostream>

#include "cs/net/json/object.hh"

namespace cs::net::json {

std::ostream& operator<<(std::ostream& os,
                         const Object* object) {
  return SerializeObject(os, object);
}

namespace {
std::ostream& WriteIndent(std::ostream& os, uint indent) {
  for (uint i = 0; i < indent; ++i) {
    os << " ";
  }
  return os;
}
}  // namespace

std::ostream& SerializeObject(std::ostream& os,
                              const Object* object) {
  return SerializeObjectPrettyPrintRecurse(os, object, 0,
                                           0);
}

std::ostream& SerializeObjectPrettyPrintRecurse(
    std::ostream& os, const Object* object, uint indent,
    uint initial_indent) {
  if (object->_type == Type::BOOLEAN) {
    if (object->as_bool()) {
      os << "true";
    } else {
      os << "false";
    }
  } else if (object->_type == Type::FLOAT) {
    os << object->as_number();
  } else if (object->_type == Type::STRING) {
    os << '"' << object->as_string() << '"';
  } else if (object->_type == Type::ARRAY) {
    os << "[";
    if (indent > 0) {
      os << "\n";
    }
    bool first = true;
    for (const auto& elem : object->as_array()) {
      if (!first) {
        os << ",";
        if (indent > 0) {
          os << "\n";
        }
      } else {
        first = false;
      }
      WriteIndent(os, initial_indent + indent);
      SerializeObjectPrettyPrintRecurse(
          os, elem, indent, initial_indent + indent);
    }
    if (indent > 0) {
      os << "\n";
    }
    WriteIndent(os, initial_indent);
    os << "]";
  } else if (object->_type == Type::MAP) {
    os << "{";
    if (indent > 0) {
      os << "\n";
    }
    bool first = true;
    for (const auto& kv : object->as_map()) {
      if (!first) {
        os << ",";
        if (indent > 0) {
          os << "\n";
        }
      } else {
        first = false;
      }
      WriteIndent(os, initial_indent + indent);
      os << '"' << kv.first << '"';
      os << ":";
      if (indent > 0) {
        os << " ";
      }
      SerializeObjectPrettyPrintRecurse(
          os, kv.second, indent, initial_indent + indent);
    }
    if (indent > 0) {
      os << "\n";
    }
    WriteIndent(os, initial_indent);
    os << "}";
  }
  return os;
};

}  // namespace cs::net::json