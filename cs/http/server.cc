
#include "cs/http/server.hh"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "cs/http/request.hh"
#include "cs/profiling/time_it.hh"
#include "cs/result/result.hh"
#include "cs/sanity/ensure.hh"
#include "cs/sanity/error.hh"

// Based on: https://github.com/OsasAzamegbe/http-server
namespace {

const unsigned int BUFFER_SIZE = 2 << 16;
#define VERBOSE_LOG false

using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

}  // namespace

namespace cs::http {

Server::Server(std::string ip_address, int port)
    : _ip_address(ip_address),
      _port(port),
      _socket_address_length(sizeof(_socket_address)) {
  _socket_address.sin_family = AF_INET;
  _socket_address.sin_port = htons(_port);
  _socket_address.sin_addr.s_addr =
      inet_addr(_ip_address.c_str());
}

Server::~Server() { closeServer(); }

Result Server::startServer() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  ENSURE(_socket >= 0);

  ENSURE(bind(_socket, (sockaddr *)&_socket_address,
              _socket_address_length) >= 0);

  return Ok();
}

Result Server::closeServer() {
  close(_socket);
  close(_response_socket);
  return Ok();
}

Result Server::startListening(
    std::function<Response(Request)> request_handler) {
  ENSURE(listen(_socket, 20) >= 0);

  std::cout << "Listening on "
            << inet_ntoa(_socket_address.sin_addr) << ":"
            << ntohs(_socket_address.sin_port) << std::endl;

  while (true) {
    _response_socket =
        accept(_socket, (sockaddr *)&_socket_address,
               &_socket_address_length);

    Response response;
    bool success = false;
    unsigned int processing_time_ms =
        cs::profiling::time_it([&response, request_handler,
                                this, &success]() {
          ENSURE(_response_socket >= 0);

          char buffer[BUFFER_SIZE] = {0};
          int bytesReceived =
              read(_response_socket, buffer, BUFFER_SIZE);

          ENSURE(bytesReceived >= 0);

#if VERBOSE_LOG
          std::cout << "<<< NEW REQUEST <<<<<<<<<<<<<<<<"
                    << std::endl
                    << buffer << std::endl
                    << "================================"
                    << std::endl;
#endif  // VERBOSE_LOG

          Request request;
          Result parse_result = buffer >> request;
          if (!parse_result.ok()) {
            success = false;
            std::cerr << parse_result.message()
                      << std::endl;
            return;
          }
          response = request_handler(request);
          success = true;
        });

    if (success) {
      std::stringstream ss;
#define APPEND_SERVER_STATS true
#if APPEND_SERVER_STATS
      ss << response.body() << std::endl
         << "<hr/>Processed in " << processing_time_ms
         << " ms.";
#endif
      response = Response(response.status(),
                          kContentTypeTextHtml, ss.str());
      std::string response_str = response.to_string();

      unsigned long bytesSent =
          write(_response_socket, response_str.c_str(),
                response_str.size());

#if VERBOSE_LOG
      std::cout << ">>> SENDING RESPONSE >>>>>>>>>>>"
                << std::endl
                << response << std::endl
                << "================================"
                << std::endl;
#endif  // VERBOSE_LOG

      if (bytesSent != response_str.size()) {
        std ::cerr << "Failed to send out " << bytesSent
                   << "." << std::endl;
      }
    }

    close(_response_socket);
  }

  return Ok();
}

}  // namespace cs::http
