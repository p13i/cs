#ifndef CS_RESULT_RESULT_HH
#define CS_RESULT_RESULT_HH

#include <stdio.h>

#include <iostream>
#include <string>

#define ASSIGN_OR_RETURN(var, result_or) \
  {                                      \
    const auto eval__ = result_or;       \
    if (eval__.result().ok()) {          \
      var = eval__.data();               \
    } else {                             \
      return eval__.result();            \
    }                                    \
  }

#define OK_OR_RETURN(result)                             \
  {                                                      \
    const auto eval__ = result;                          \
    if (!eval__.ok()) {                                  \
      std::cerr << "OK_OR_RETURN failed at " << __FILE__ \
                << ":" << __LINE__ << ". " << #result    \
                << ": " << eval__ << std::endl;          \
      return eval__;                                     \
    }                                                    \
  }

namespace cs::result {
class Result {
 public:
  Result(bool ok, std::string message)
      : _ok(ok), _message(message) {
    if (_ok) {
      _code = 0;
    } else {
      _code = 1;
    }
  }

  bool ok() const { return _ok; }

  std::string message() const { return _message; }

  int code() const { return _code; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Result& result) {
    return os << "Result(" << (result._ok ? "OK" : "ERROR")
              << ", code=" << result._code
              << ", message=" << result._message << ")";
  }

 private:
  bool _ok;
  int _code;
  std::string _message;
};

class Ok : public Result {
 public:
  Ok() : Result(true, "") {}
  Ok(std::string message) : Result(true, message) {}
};

class Error : public Result {
 public:
  Error(const std::string& message)
      : Result(false, message) {}
};

// ResultOr class with a template parameter for data
template <typename Data>
class ResultOr : public Result {
 public:
  ResultOr(const Data& data)
      : Result(Ok()), _result(Ok()), _data(data) {}

  ResultOr(const Error& result)
      : Result(result), _result(result) {}

  ResultOr(const Result& result)
      : Result(result), _result(result) {}

  ResultOr(const Result& result, const Data& data)
      : Result(result), _result(result), _data(data) {}

  bool ok() const { return _result.ok(); }
  Data data() const { return _data; }
  Data value() const { return _data; }
  Result result() const { return _result; }
  Data value_or(Data default_value) const {
    if (ok()) {
      return _data;
    }
    return default_value;
  }

  Data operator->() { return _data; }

  friend std::ostream& operator<<(
      std::ostream& os, const ResultOr<Data>& result) {
    return os << "ResultOr("
              << (result.ok() ? "OK" : "ERROR")
              << ", code=" << result.code()
              << ", message=" << result.message() << ")";
  }

 private:
  Result _result;
  Data _data;
};

}  // namespace cs::result

#endif  // CS_RESULT_RESULT_HH