#ifndef CS_DB_QUERY_VIEW_HH
#define CS_DB_QUERY_VIEW_HH

#include <functional>
#include <vector>

#include "cs/result/result.hh"

namespace {
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;
using ::cs::result::ResultOr;
}  // namespace

template <typename DataType>
class QueryView {
  typedef std::vector<DataType> Ts;

 public:
  QueryView(const Ts& values) : _values(values) {}

  Ts values() { return _values; }

  QueryView WHERE(std::function<bool(DataType)> lambda) {
    Ts matching;
    for (const auto& item : _values) {
      if (lambda(item)) {
        matching.push_back(item);
      }
    }
    return matching;
  }

  QueryView ORDER_BY(
      std::function<bool(DataType, DataType)> comparator) {
    Ts sorted(_values);
    std::sort(sorted.begin(), sorted.end(), comparator);
    return sorted;
  }
  DataType FIRST() { return _values[0]; }
  bool Any() { return _values.size() > 0; }

 private:
  Ts _values;
};

#endif  // CS_DB_QUERY_VIEW_HH