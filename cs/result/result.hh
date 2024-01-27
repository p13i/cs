#ifndef CS_RESULT_RESULT_HH
#define CS_RESULT_RESULT_HH

#include <string>

#define ENSURE_OK(result) \
  if (!result.ok()) {     \
    return result;        \
  }

namespace cs::result {
class Result {
 public:
  Result(bool ok, std::string message)
      : _ok(ok), _message(message) {}

  bool ok() { return _ok; }

  std::string message() { return _message; }

 private:
  bool _ok;
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