#ifndef CS_HTTP_WEB_APP_HH
#define CS_HTTP_WEB_APP_HH

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>

#include "cs/http/request.hh"
#include "cs/http/response.hh"
#include "cs/http/server.hh"
#include "cs/result/result.hh"

namespace cs::http {
namespace {

using ::cs::http::kContentTypeTextHtml;
using ::cs::http::Request;
using ::cs::http::Response;
typedef std::function<Response(Request)> RequestHandler;

// https://stackoverflow.com/a/63864750
std::string NowAsISO8601TimeUTC() {
  auto now = std::chrono::system_clock::now();
  auto itt = std::chrono::system_clock::to_time_t(now);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&itt), "%FT%TZ");
  return ss.str();
}

bool PathMatches(std::string app_path,
                 std::string user_path) {
  return app_path == user_path;
}

}  // namespace

class WebApp {
 public:
  Result Register(std::string method, std::string path,
                  RequestHandler handler) {
    _handlers.push_back(
        std::make_tuple(method, path, handler));
    return Ok();
  }

  Result RunServer(std::string ip_address, int port) {
    auto server = cs::http::Server(ip_address, port);
    ENSURE_OK(server.startServer());
    ENSURE_OK(server.startListening(
        std::bind(&WebApp::main_handler, this,
                  std::placeholders::_1)));
    return Ok();
  }

 private:
  Response main_handler(Request request) {
    std::cout << ">>> [" << NowAsISO8601TimeUTC() << "] "
              << request.method() << " " << request.path()
              << std::endl;
    Response response;
    bool found_handler = false;
    for (auto path_info : _handlers) {
      const auto [method, path, handler] = path_info;
      if (request.method() == method &&
          PathMatches(request.path(), path)) {
        found_handler = true;
        response = handler(request);
        break;
      }
    }
    if (!found_handler) {
      response = Response(HTTP_404_NOT_FOUND);
    }
    std::cout << "<<< [" << NowAsISO8601TimeUTC() << "] "
              << response.status() << std::endl;
    return response;
  };

  std::vector<
      std::tuple<std::string, std::string, RequestHandler>>
      _handlers;
};
}  // namespace cs::http

#endif  // CS_HTTP_WEB_APP_HH