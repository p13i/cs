#ifndef CS_NET_JSON_SERIALIZE_HH
#define CS_NET_JSON_SERIALIZE_HH

#include <iostream>

#include "cs/net/json/object.hh"

namespace cs::net::json {
std::ostream& operator<<(std::ostream& os,
                         const Object* object);

std::ostream& SearializeObject(std::ostream& os,
                               const Object* object);

std::ostream& SearializeObject(std::ostream& os,
                               const Object* object,
                               uint indent = 0,
                               uint initial_indent = 0);
}  // namespace cs::net::json

#endif  // CS_NET_JSON_SERIALIZE_HH