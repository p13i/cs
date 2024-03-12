#include "cs/db/in_memory_database.hh"

#include "cs/db/database.hh"
#include "cs/db/table.hh"
#include "cs/result/result.hh"

namespace cs::db {
namespace {
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

template <typename DataType>
Result InMemoryDatabase::LoadTable(DataType data_type,
                                   std::string type_name) {
  return Ok();
}
template <typename DataType>
Result InMemoryDatabase::SaveTable(DataType data_type,
                                   std::string type_name) {
  return Ok();
}
template <typename DataType>
ResultOr<Table<DataType>> InMemoryDatabase::GetTable(
    std::string name) {
  return Table<DataType>();
}
}  // namespace cs::db
