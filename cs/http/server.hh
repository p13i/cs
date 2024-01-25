#ifndef CS_HTTP_SERVER_HH
#define CS_HTTP_SERVER_HH

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <functional>
#include <string>

namespace cs::http {

// Based on: https://github.com/OsasAzamegbe/http-server
class Server {
 public:
  Server(std::string ip_address, int port);
  ~Server();
  int startListening(std::function<std::string(std::string)>
                         request_handler);

 private:
  std::string _ip_address;
  int _port;
  int _socket;
  int _response_socket;
  long _incomingMessage;
  struct sockaddr_in _socketAddress;
  unsigned int _socketAddress_len;
  std::string _serverMessage;

  int startServer();
  void closeServer();
};

}  // namespace cs::http

#endif  // CS_HTTP_SERVER_HH