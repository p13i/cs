#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cs/db/models/user.hh"
#include "cs/db/query_view.hh"
#include "cs/db/table.hh"
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
