#include <iostream>
#include <vector>

#include "cs/db/models/user.hh"
#include "cs/db/query_view.hh"
#include "cs/db/table.hh"
#include "cs/result/result.hh"

using ::cs::db::Table;
using ::cs::db::models::User;

#define DESC(key) \
  [](const auto& a, const auto& b) { return a.key > b.key; }

#define ASC(key) \
  [](const auto& a, const auto& b) { return a.key < b.key; }

int main() {
  std::vector<User> users{
      {1, "email1@example.com", "Rob B"},
      {2, "email2@example.com", "Tom B"},
      {3, "email3@example.com", "Hayden B"},
      {4, "email3@example.com", "Paris C"},
  };

  // Build a table.
  auto table = Table<User>(users);

  std::string filepath =
      "/workspaces/cs/users.json";  // distribute this bitch
#if 1
  cs::result::Result result = table.load(filepath);
  if (!result.ok()) {
    std::cerr << result << std::endl;
    return result.code();
  }
#endif

  // Query the table.
  std::vector<User> results =
      table.query_view()
          .where(
              [](User u) { return u.full_name.size() > 1; })
          .order_by(ASC(id))
          .limit(2)
          .values();

  // Write runtime db to filesystem.
  Result save_result = table.save(filepath);
  if (!save_result.ok()) {
    std::cerr << save_result.message() << std::endl;
    return save_result.code();
  }
  return 0;
}
