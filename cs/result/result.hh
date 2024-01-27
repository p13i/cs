#ifndef CS_RESULT_RESULT_HH
#define CS_RESULT_RESULT_HH

#include <string>

namespace cs::result {
class Result {
 public:
  Result(bool ok, std::string message)
      : _ok(ok), _message(message) {}

 private:
  bool _ok;
  std::string _message;
};

class Ok : public Result {
  Ok() : Result(true, "") {}
};

class Error : public Result {
  Error(std::string message) : Result(false, message) {}
}

}  // namespace cs::result

#endif  // CS_RESULT_RESULT_HH