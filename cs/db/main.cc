#include <iostream>
#include <vector>

#include "cs/db/models/user.hh"
#include "cs/db/query_view.hh"
#include "cs/db/table.hh"

int main() {
  // Fake data.
  std::vector<User> users{
      {1, "Email", "Name"},
      {2, "Email2", "Name2"},
  };

  // Build a table.
  auto table = Table<User>(users);

  // Query the table.
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
