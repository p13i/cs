#include <stdint.h>

#include <string>

namespace cs::http {
class Request {
 public:
  void parse(std::string bytes);

 private:
  std::string _method;
  std::string _path;
  std::string _body;
};

class Response {
 public:
  std::string to_string() { return ""; }

 private:
  uint8_t _status;
  std::string _html;
};
}  // namespace cs::http