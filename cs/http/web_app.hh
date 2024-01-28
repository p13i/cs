#ifndef CS_HTTP_WEB_APP_HH
#define CS_HTTP_WEB_APP_HH

#include <functional>

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
    for (auto path_info : _handlers) {
      const auto [method, path, handler] = path_info;
      if (request.method() == method &&
          request.path() == path) {
        return handler(request);
      }
    }
    return Response(HTTP_404_NOT_FOUND);
  };

  std::vector<
      std::tuple<std::string, std::string, RequestHandler>>
      _handlers;
};
}  // namespace cs::http

#endif  // CS_HTTP_WEB_APP_HH