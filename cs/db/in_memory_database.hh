#ifndef CS_DB_IN_MEMORY_DATABASE_HH
#define CS_DB_IN_MEMORY_DATABASE_HH

#include "cs/db/database.hh"
#include "cs/db/table.hh"

namespace cs::db {
namespace {
using ::cs::db::Table;
}

class InMemoryDatabase : public Database {
 public:
  template <typename DataType>
  Result LoadTable(DataType data_type,
                   std::string type_name);
  template <typename DataType>
  Result SaveTable(DataType data_type,
                   std::string type_name);
  template <typename DataType>
  ResultOr<Table<DataType>> GetTable(std::string name);
};
}  // namespace cs::db

#endif  // CS_DB_IN_MEMORY_DATABASE_HH