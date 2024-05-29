#ifndef CS_NET_HTTP_SERVER_HH
#define CS_NET_HTTP_SERVER_HH

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <functional>
#include <string>

#include "cs/net/http/request.hh"
#include "cs/net/http/response.hh"
#include "cs/result/result.hh"

namespace cs::net::http {

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
  std::string uuid_;
};

}  // namespace cs::net::http

#endif  // CS_NET_HTTP_SERVER_HH