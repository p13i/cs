#ifndef CS_NET_JSON_SERIALIZE_HH
#define CS_NET_JSON_SERIALIZE_HH

#include <iostream>

#include "cs/net/json/object.hh"

namespace cs::net::json {
std::ostream& SearializeObject(std::ostream& os,
                               const Object* object);
}

#endif  // CS_NET_JSON_SERIALIZE_HH