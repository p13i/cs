#ifndef CS_DB_TABLE_HH
#define CS_DB_TABLE_HH

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cs/db/query_view.hh"
#include "cs/result/result.hh"

namespace cs::db {
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
  Result Insert(const DataType& value) {
    _values.push_back(value);
    return Ok();
  }
};
}  // namespace cs::db

#endif  // CS_DB_TABLE_HH