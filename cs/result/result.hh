#ifndef CS_RESULT_RESULT_HH
#define CS_RESULT_RESULT_HH

#include <stdio.h>

#include <iostream>
#include <string>

#define ASSIGN_OR_RETURN(var, result_or) \
  {                                      \
    const auto eval = result_or;         \
    if (eval.result().ok()) {            \
      var = eval.data();                 \
    } else {                             \
      return eval.result();              \
    }                                    \
  }

#define MOVE_OR_RETURN(var, result_or)   \
  {                                      \
    if (result_or.result().ok()) {       \
      var = std::move(result_or.data()); \
    } else {                             \
      return result_or.result();         \
    }                                    \
  }

#define OK_OR_RETURN(result)                             \
  {                                                      \
    if (!result.ok()) {                                  \
      std::cerr << "OK_OR_RETURN failed at " << __FILE__ \
                << ":" << __LINE__ << ". " << #result    \
                << ": " << result << std::endl;          \
      return result;                                     \
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