#ifndef CS_DB_TABLE_HH
#define CS_DB_TABLE_HH

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cs/db/query_view.hh"
#include "cs/net/json/object.hh"
#include "cs/net/json/parsers.hh"
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

  Result insert(const DataType& value) {
    _values.push_back(value);
    return Ok();
  }

  Result save(const std::string& path) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
      return Error("Failed to open file `" + path + "`.");
    }
    // Serialize the data.
    for (const DataType& value : _values) {
      file << value.JsonSerialize() << "\n";
    }
    file.close();
    return Ok();
  }

  Result load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
      return Error("Failed to open file.");
    }
    std::string line;
    std::vector<DataType> data;
    while (std::getline(file, line)) {
      std::cout << "Reading: " << line << "\n";
      DataType datum;
      OK_OR_RETURN(datum.JsonParse(line));
      data.push_back(datum);
    }
    _values = data;
    return Ok();
  }
};
}  // namespace cs::db

#endif  // CS_DB_TABLE_HH