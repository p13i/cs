#ifndef CS_NET_HTTP_WEB_APP_HH
#define CS_NET_HTTP_WEB_APP_HH

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>

#include "cs/net/http/request.hh"
#include "cs/net/http/response.hh"
#include "cs/net/http/server.hh"
#include "cs/result/result.hh"

namespace cs::net::http {
namespace {

using ::cs::net::http::kContentTypeTextHtml;
using ::cs::net::http::Request;
using ::cs::net::http::Response;
typedef std::function<Response(Request)> RequestHandler;

// https://stackoverflow.com/a/63864750
std::string NowAsISO8601TimeUTC() {
  auto now = std::chrono::system_clock::now();
  auto itt = std::chrono::system_clock::to_time_t(now);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&itt), "%FT%TZ");
  return ss.str();
}

}  // namespace

class WebApp {
 public:
  Result Register(std::string method, std::string path,
                  RequestHandler handler) {
    handlers_.push_back(
        std::make_tuple(method, path, handler));
    return Ok();
  }

  Result RunServer(std::string ip_address, int port) {
    auto server = cs::net::http::Server(ip_address, port);
    OK_OR_RETURN(server.startServer());
    OK_OR_RETURN(server.startListening(
        std::bind(&WebApp::main_handler, this,
                  std::placeholders::_1)));
    return Ok();
  }

  // Return routes
  std::vector<std::tuple<std::string, std::string>>
  Routes() {
    std::vector<std::tuple<std::string, std::string>>
        routes;
    for (auto path_info : handlers_) {
      const auto [method, path, handler] = path_info;
      routes.push_back(std::make_tuple(method, path));
    }
    return routes;
  }

 private:
  Response main_handler(Request request) {
    std::cout << ">>> [" << NowAsISO8601TimeUTC() << "] "
              << request.method() << " " << request.path()
              << std::endl;
    Response response(HTTP_404_NOT_FOUND);
    for (auto path_info : handlers_) {
      const auto [method, path, handler] = path_info;
      if (request.method() == method &&
          request.path() == path) {
        response = handler(request);
        break;
      }
    }
    std::cout << "<<< [" << NowAsISO8601TimeUTC() << "] "
              << response.status() << std::endl;
    return response;
  };

  std::vector<
      std::tuple<std::string, std::string, RequestHandler>>
      handlers_;
};
}  // namespace cs::net::http

#endif  // CS_NET_HTTP_WEB_APP_HH
