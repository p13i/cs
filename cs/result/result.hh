#ifndef CS_RESULT_RESULT_HH
#define CS_RESULT_RESULT_HH

#include <stdio.h>

#include <string>

#define ENSURE_OK(result)                                  \
  if (!result.ok()) {                                      \
    std::cerr << "ENSURE_OK failed at " << __FILE__ << ":" \
              << __LINE__ << ". " << #result << ": "       \
              << result << std::endl;                      \
    return result;                                         \
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

  bool ok() { return _ok; }

  std::string message() { return _message; }

  int code() { return _code; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Result& result) {
    std::string ok_str;
    if (result._ok) {
      ok_str = "OK";
    } else {
      ok_str = "ERROR";
    }
    return os << "Result(" << ok_str
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
  Error(std::string message) : Result(false, message) {}
};

}  // namespace cs::result

#endif  // CS_RESULT_RESULT_HH