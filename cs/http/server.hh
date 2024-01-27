#ifndef CS_HTTP_SERVER_HH
#define CS_HTTP_SERVER_HH

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <functional>
#include <string>

#include "cs/http/request.hh"
#include "cs/http/response.hh"
#include "cs/result/result.hh"

namespace cs::http {

// Based on: https://github.com/OsasAzamegbe/http-server
class Server {
 public:
  Server(std::string ip_address, int port);
  ~Server();
  Result startListening(
      std::function<Response(Request)> request_handler);

  Result startServer();
  Result closeServer();

 private:
  std::string _ip_address;
  int _port;
  int _socket;
  int _response_socket;
  struct sockaddr_in _socket_address;
  unsigned int _socket_address_length;
};

}  // namespace cs::http

#endif  // CS_HTTP_SERVER_HH