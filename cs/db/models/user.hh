#ifndef CS_DB_MODELS_USER_HH
#define CS_DB_MODELS_USER_HH

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cs/net/json/object.hh"
#include "cs/net/json/parsers.hh"
#include "cs/net/json/serialize.hh"
#include "cs/result/result.hh"

namespace cs::db::models {
namespace {
using ::cs::net::json::Object;
using ::cs::net::json::SerializeObject;
using ::cs::net::json::Type;
using ::cs::net::json::parsers::ParseObject;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

struct User {
  float id = 0;
  std::string email = "";
  std::string full_name = "";

  User() {}
  User(float id, std::string email, std::string full_name)
      : id(id), email(email), full_name(full_name) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  const User& user) {
    auto object = new Object({
        {"id", new Object(user.id)},
        {"email", new Object(user.email)},
        {"full_name", new Object(user.full_name)},
    });
    SerializeObject(os, object);
    delete object;
    return os;
  }

  friend Result operator>>(std::istream& is, User& user) {
    std::stringstream ss;
    ss << is.rdbuf();
    return user.JsonParse(ss.str());
  }

  Result JsonParse(std::string str) {
    Object* object;
    ASSIGN_OR_RETURN(object, ParseObject(str));
    ASSIGN_OR_RETURN(id, object->get("id", float()));
    ASSIGN_OR_RETURN(email,
                     object->get("email", std::string()));
    ASSIGN_OR_RETURN(
        full_name, object->get("full_name", std::string()));
    return Ok();
  }
};
}  // namespace cs::db::models

#endif  // CS_DB_MODELS_USER_HH