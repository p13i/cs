#include <iostream>
#include <vector>

#include "cs/db/models/user.hh"
#include "cs/db/query_view.hh"
#include "cs/db/table.hh"

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
