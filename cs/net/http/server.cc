
#include "cs/net/http/server.hh"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "cs/net/http/request.hh"
#include "cs/profiling/time_it.hh"
#include "cs/result/result.hh"

// Based on: https://github.com/OsasAzamegbe/http-server
namespace {

const unsigned int BUFFER_SIZE = 2 << 16;
#define VERBOSE_LOG true

using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

}  // namespace

namespace cs::net::http {

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
  if (_socket < 0) {
    return Error("Failed to get socket.");
  }

  if (bind(_socket, (sockaddr *)&_socket_address,
           _socket_address_length) < 0) {
    return Error("Failed to bind socket.");
  }

  return Ok();
}

Result Server::closeServer() {
  close(_socket);
  close(_response_socket);
  return Ok();
}

Result Server::startListening(
    std::function<Response(Request)> request_handler) {
  if (listen(_socket, 20) < 0) {
    return Error("Failed to listen on socket.");
  }

  std::cout << "Listening on "
            << inet_ntoa(_socket_address.sin_addr) << ":"
            << ntohs(_socket_address.sin_port) << "\n";

  while (true) {
    _response_socket =
        accept(_socket, (sockaddr *)&_socket_address,
               &_socket_address_length);

    Response response;
    Result result = Ok();
    [[maybe_unused]] unsigned int processing_time_ms =
        cs::profiling::time_it([&response, request_handler,
                                this, &result]() {
          if (_response_socket < 0) {
            result = Error("Failed to accept connection.");
            return;
          }

          char buffer[BUFFER_SIZE] = {0};
          int bytesReceived =
              read(_response_socket, buffer, BUFFER_SIZE);

          if (bytesReceived < 0) {
            result = Error("Failed to read from socket.");
            return;
          }

          Request request;
          Result parse_result = buffer >> request;
          if (!parse_result.ok()) {
            result = parse_result;
            return;
          }

#if VERBOSE_LOG
          std::cout << "<<< NEW REQUEST <<<<<<<<<<<<<<<<"
                    << "\n"
                    << request << "\n"
                    << "================================"
                    << "\n";
#endif  // VERBOSE_LOG

          response = request_handler(request);
          result = Ok();
        });

    if (result.ok()) {
#define APPEND_SERVER_STATS true
#if APPEND_SERVER_STATS
      std::stringstream ss;
      ss << response.body() << "\n"
         << "<hr/>Processed in " << processing_time_ms
         << " ms.";
      response = Response(response.status(),
                          kContentTypeTextHtml, ss.str());
#endif
      std::string response_str = response.to_string();

      unsigned long bytesSent =
          write(_response_socket, response_str.c_str(),
                response_str.size());

#if VERBOSE_LOG
      std::cout << ">>> SENDING RESPONSE >>>>>>>>>>>"
                << "\n"
                << response << "\n"
                << "================================"
                << "\n";
#endif  // VERBOSE_LOG

      if (bytesSent != response_str.size()) {
        std ::cerr << "Failed to send out " << bytesSent
                   << "."
                   << "\n";
      }
    }

    close(_response_socket);
  }

  return Ok();
}

}  // namespace cs::net::http
