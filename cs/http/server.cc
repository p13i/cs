
#include "cs/http/server.hh"

#include <unistd.h>

#include <iostream>
#include <sstream>

// Based on: https://github.com/OsasAzamegbe/http-server
namespace {
const int BUFFER_SIZE = 30720;

void exitWithError(const std::string &errorMessage) {
  std::cout << "ERROR: " + errorMessage << std::endl;
  exit(1);
}

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

HttpServer::HttpServer(std::string ip_address, int port)
    : _ip_address(ip_address),
      _port(port),
      _socket(),
      _new_socket(),
      _incomingMessage(),
      _socketAddress(),
      _socketAddress_len(sizeof(_socketAddress)) {
  _socketAddress.sin_family = AF_INET;
  _socketAddress.sin_port = htons(_port);
  _socketAddress.sin_addr.s_addr =
      inet_addr(_ip_address.c_str());

  if (startServer() != 0) {
    std::ostringstream ss;
    ss << "Failed to start server with PORT: "
       << ntohs(_socketAddress.sin_port);
    std::cout << ss.str() << std::endl;
  }
}

HttpServer::~HttpServer() { closeServer(); }

int HttpServer::startServer() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket < 0) {
    exitWithError("Cannot create socket");
    return 1;
  }

  if (bind(_socket, (sockaddr *)&_socketAddress,
           _socketAddress_len) < 0) {
    exitWithError("Cannot connect socket to address");
    return 1;
  }

  return 0;
}

void HttpServer::closeServer() {
  close(_socket);
  close(_new_socket);
  exit(0);
}

int HttpServer::startListening(
    std::function<std::string(std::string)>
        request_handler) {
  if (listen(_socket, 20) < 0) {
    exitWithError("Socket listen failed");
  }

  std::cout << "Listening on "
            << inet_ntoa(_socketAddress.sin_addr) << ":"
            << ntohs(_socketAddress.sin_port) << "."
            << std::endl;

  while (true) {
    _new_socket =
        accept(_socket, (sockaddr *)&_socketAddress,
               &_socketAddress_len);

    if (_new_socket < 0) {
      std::ostringstream ss;
      ss << "Server failed to accept incoming connection "
            "from ADDRESS: "
         << inet_ntoa(_socketAddress.sin_addr)
         << "; PORT: " << ntohs(_socketAddress.sin_port);
      exitWithError(ss.str());
    }

    char buffer[BUFFER_SIZE] = {0};
    int bytesReceived =
        read(_new_socket, buffer, BUFFER_SIZE);
    if (bytesReceived < 0) {
      exitWithError(
          "Failed to read bytes from client socket "
          "connection");
    }

    std::cout << buffer << std::endl;

    std::string response = WrapHttpResponse(
        request_handler(std::string(buffer)));

    long bytesSent = write(_new_socket, response.c_str(),
                           response.size());

    std::cout << response << std::endl;

    if (bytesSent == response.size()) {
      std::cout << "Sent " << bytesSent
                << " bytes to client." << std::endl;
    } else {
      exitWithError("Error sending response to client");
    }

    close(_new_socket);
  }

  return 0;
}

}  // namespace cs::http