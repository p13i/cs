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
using ::cs::net::json::SearializeObject;
using ::cs::net::json::Type;
using ::cs::net::json::parsers::ParseObject;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

struct User {
 public:
  User() {}
  User(int id, std::string email, std::string full_name)
      : id_(id), email_(email), full_name_(full_name) {}
  int id() const { return id_; }
  std::string email() const { return email_; }
  std::string full_name() const { return full_name_; }

  std::string Serialize() {
    std::stringstream ss;
    auto obj = new Object({
        {"id", new Object(static_cast<float>(id_))},
        {"email", new Object(email_)},
        {"full_name", new Object(full_name_)},
    });
    SearializeObject(ss, obj);
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const User& user) {
    return os << "User{id=" << user.id_
              << ", email=" << user.email_
              << ", full_name=" << user.full_name_ << "}";
  }

  Result Parse(std::string str) {
    Object* object;
    uint cursor = 0;
    ASSIGN_OR_RETURN(object, ParseObject(str, &cursor));
    if (object->type() != Type::MAP) {
      return Error("Failed to parse string into map.");
    }
    auto map = object->as_map();
    // Load id
    if (map.find("id") == map.end() ||
        map["id"]->type() != Type::NUMBER) {
      return Error("Failed to parse id as float.");
    }
    id_ = map["id"]->as_number();
    // load email
    if (map.find("email") == map.end() ||
        map["email"]->type() != Type::STRING) {
      return Error("Failed to parse email as string.");
    }
    email_ = map["email"]->as_string();
    // load full_name
    if (map.find("full_name") == map.end() ||
        map["full_name"]->type() != Type::STRING) {
      return Error("Failed to parse full_name as string.");
    }
    full_name_ = map["full_name"]->as_string();
    return Ok();
  }

 private:
  int id_;
  std::string email_;
  std::string full_name_;
};
}  // namespace cs::db::models

#endif  // CS_DB_MODELS_USER_HH