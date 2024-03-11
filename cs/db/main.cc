#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cs/net/json/object.hh"
#include "cs/net/json/parsers.hh"
#include "cs/net/json/serialize.hh"
#include "cs/result/result.hh"

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

template <typename DataType>
class QueryView {
  typedef std::vector<DataType> Ts;

 public:
  QueryView(const Ts& values) : _values(values) {}

  Ts values() { return _values; }

  QueryView Where(std::function<bool(DataType)> lambda) {
    Ts matching;
    for (const auto& item : _values) {
      if (lambda(item)) {
        matching.push_back(item);
      }
    }
    return matching;
  }

  QueryView OrderBy(
      std::function<bool(DataType, DataType)> comparator) {
    Ts sorted(_values);
    std::sort(sorted.begin(), sorted.end(), comparator);
    return sorted;
  }
  DataType First() { return _values[0]; }
  bool Any() { return _values.size() > 0; }

 private:
  Ts _values;
};

template <typename DataType>
class Table {
  typedef std::vector<DataType> Ts;

 private:
  Ts _values;

 public:
  Table() {}
  Table(const Ts& values) : _values(values) {}
  QueryView<DataType> query_view() {
    return QueryView<DataType>(_values);
  }
};

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

int main() {
  std::vector<User> users{
      {1, "Email", "Name"},
      {2, "Email2", "Name2"},
  };
  auto table = Table<User>(users);
  auto view = table.query_view()
                  .Where([](User u) {
                    return u.full_name().size() > 1;
                  })
                  .OrderBy([](User a, User b) {
                    return a.id() > b.id();
                  });

  // Print results.
  for (const auto& user : view.values()) {
    std::cout << user << std::endl;
  }
}
