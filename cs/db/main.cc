#include <iostream>
#include <string>
#include <vector>

#include "cs/db/models/user.hh"
#include "cs/db/query_view.hh"
#include "cs/db/table.hh"
#include "cs/result/result.hh"
#include "cs/string/format.h"

using ::cs::db::Table;
using ::cs::db::models::User;

#define LAMBDA(type, predicate) \
  [](const auto& type) { return predicate; }, #predicate

#define DESCENDING(key)              \
  [](const auto& a, const auto& b) { \
    return a.key > b.key;            \
  },                                 \
      "DESCENDING(key)"

#define ASCENDING(key)                \
  ([](const auto& a, const auto& b) { \
    return a.key < b.key;             \
  }),                                 \
      "ASCENDING(key)"

Result RunDatabase() {
  std::vector<User> users{
      User(1, "email1@example.com", "Rob O"),
      User(2, "email2@example.com", "Tom B"),
      User(3, "email3@example.com", "Hayden"),
      User(4, "email3@example.com", "Scott"),
      User(4, "email3@example.com", "Jamie"),
      User(4, "email3@example.com", "Ben"),
  };

  // Build a table.
  auto table = Table<User>(users);

  std::string filepath =
      "/workspaces/cs/users.json";  // distribute this bitch
  OK_OR_RETURN(table.load(filepath));
  for (const auto& user : users) {
    OK_OR_RETURN(table.insert(user));
  }
  // Print all items
  for (const auto& item : table.query_view().values()) {
    std::cout << item << std::endl;
  }
  // table.load(filepath);
  // Query the table.
  auto query_view = table.query_view();
  std::vector<User> results =
      query_view
          .where(LAMBDA(user, user.full_name.size() > 1))
          .order_by(ASCENDING(id))
          .limit(2)
          .values();

  std::cout << "Results: `" << query_view.query_string()
            << "`\n";
  for (const auto& item : results) {
    std::cout << item << "\n";
  }

  // Write runtime db to filesystem.
  OK_OR_RETURN(
      table.insert({6, "pk@mail.com", "Pramod K"}));
  OK_OR_RETURN(table.save(filepath));
  return Ok();
}

int main() {
  Result result = RunDatabase();
  if (!result.ok()) {
    std::cerr << result.message() << std::endl;
  }
  return result.code();
}
