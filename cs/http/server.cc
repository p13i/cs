
#include "cs/http/server.hh"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "cs/http/request.hh"
#include "cs/sanity/ensure.hh"
#include "cs/sanity/error.hh"

// Based on: https://github.com/OsasAzamegbe/http-server
namespace {

const unsigned int BUFFER_SIZE = 2 << 16;

std::string WrapHttpResponse(std::string html) {
  std::ostringstream ss;
  ss << "HTTP/1.1 200 OK" << std::endl
     << "Content-Type: text/html" << std::endl
     << "Content-Length: " << html.size() << std::endl
     << std::endl
     << html;
  return ss.str();
}

}  // namespace

namespace cs::http {

Server::Server(std::string ip_address, int port)
    : _ip_address(ip_address),
      _port(port),
      _socket(),
      _response_socket(),
      _incomingMessage(),
      _socketAddress(),
      _socketAddress_len(sizeof(_socketAddress)) {
  _socketAddress.sin_family = AF_INET;
  _socketAddress.sin_port = htons(_port);
  _socketAddress.sin_addr.s_addr =
      inet_addr(_ip_address.c_str());
  ENSURE(startServer() == 0);
}

Server::~Server() { closeServer(); }

int Server::startServer() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  ENSURE(_socket >= 0);

  ENSURE(bind(_socket, (sockaddr *)&_socketAddress,
              _socketAddress_len) >= 0);

  return 0;
}

void Server::closeServer() {
  close(_socket);
  close(_response_socket);
  exit(0);
}

int Server::startListening(
    std::function<std::string(std::string)>
        request_handler) {
  ENSURE(listen(_socket, 20) >= 0);

  std::cout << "Listening on "
            << inet_ntoa(_socketAddress.sin_addr) << ":"
            << ntohs(_socketAddress.sin_port) << std::endl;

  while (true) {
    _response_socket =
        accept(_socket, (sockaddr *)&_socketAddress,
               &_socketAddress_len);

    ENSURE(_response_socket >= 0);

    char buffer[BUFFER_SIZE] = {0};
    int bytesReceived =
        read(_response_socket, buffer, BUFFER_SIZE);

    ENSURE(bytesReceived >= 0);

    std::cout << "<<< NEW REQUEST <<<<<<<<<<<<<<<<"
              << std::endl
              << buffer << std::endl
              << "================================"
              << std::endl;

    Request request(buffer);
    std::cout << request << std::endl;

    std::string response = WrapHttpResponse(
        request_handler(std::string(buffer)));

    long unsigned int bytesSent =
        write(_response_socket, response.c_str(),
              response.size());

    std::cout << ">>> SENDING RESPONSE >>>>>>>>>>>"
              << std::endl
              << response << std::endl
              << "================================"
              << std::endl;

    ENSURE(bytesSent == response.size());

    close(_response_socket);
  }

  return 0;
}

}  // namespace cs::http
