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
 private:
  float id_;
  std::string email_;
  std::string full_name_;

 public:
  User() {}
  User(float id, std::string email, std::string full_name)
      : id_(id), email_(email), full_name_(full_name) {}
  float id() const { return id_; }
  std::string email() const { return email_; }
  std::string full_name() const { return full_name_; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const User& user) {
    return os << "User{id=" << user.id_
              << ", email=" << user.email_
              << ", full_name=" << user.full_name_ << "}";
  }

  friend Result operator>>(std::istream& is, User& user) {
    std::stringstream ss;
    ss << is.rdbuf();
    return user.Parse(ss.str());
  }

  Result Parse(std::string str) {
    Object* object;
    ASSIGN_OR_RETURN(object, ParseObject(str));
    ASSIGN_OR_RETURN(id_, object->get_as("id", float()));
    ASSIGN_OR_RETURN(
        email_, object->get_as("email", std::string()));
    ASSIGN_OR_RETURN(
        full_name_,
        object->get_as("full_name", std::string()));
    return Ok();
  }

  std::string Serialize() {
    std::stringstream ss;
    auto obj = new Object({
        {"id", new Object(id_)},
        {"email", new Object(email_)},
        {"full_name", new Object(full_name_)},
    });
    SerializeObject(ss, obj);
    return ss.str();
  }
};
}  // namespace cs::db::models

#endif  // CS_DB_MODELS_USER_HH