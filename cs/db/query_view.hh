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
  std::vector<std::string> steps;
  QueryView(const Ts& values) : _values(values) {}

  std::string query_string() {
    std::stringstream ss;
    bool first = true;
    for (const auto& step : steps) {
      if (!first) {
        ss << " ";
      }
      ss << step;
    }
    ss << ";";
    return ss.str();
  }

  Ts values() { return _values; }

  QueryView where(std::function<bool(DataType)> lambda,
                  const char* literal) {
    steps.push_back(
        cs::string::format("where (%s)", literal));
    Ts matching;
    for (const auto& item : _values) {
      if (lambda(item)) {
        matching.push_back(item);
      }
    }
    return matching;
  }

  QueryView limit(uint n) {
    steps.push_back(cs::string::format("limit %d", n));
    Ts limited;
    for (uint i = 0; i < n && i < _values.size(); i++) {
      limited.push_back(_values[i]);
    }
    return limited;
  }

  QueryView order_by(
      std::function<bool(DataType, DataType)> comparator,
      const char* literal) {
    steps.push_back(
        cs::string::format("order_by (%s)", literal));
    Ts sorted(_values);
    std::sort(sorted.begin(), sorted.end(), comparator);
    return sorted;
  }

  DataType first() {
    steps.push_back(cs::string::format("first"));
    return _values[0];
  }
  bool any() { return _values.size() > 0; }

 private:
  Ts _values;
};

#endif  // CS_DB_QUERY_VIEW_HH