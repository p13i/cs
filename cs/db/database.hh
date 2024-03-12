#ifndef CS_DB_DATABASE_HH
#define CS_DB_DATABASE_HH

#include "cs/db/table.hh"
#include "cs/result/result.hh"

namespace cs::db {

namespace {
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

class Database {
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

#endif  // CS_DB_DATABASE_HH