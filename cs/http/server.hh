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
class HttpServer {
 public:
  HttpServer(std::string ip_address, int port);
  ~HttpServer();
  int startListening(std::function<void()> request_handler);

 private:
  std::string _ip_address;
  int _port;
  int _socket;
  int _new_socket;
  long _incomingMessage;
  struct sockaddr_in _socketAddress;
  unsigned int _socketAddress_len;
  std::string _serverMessage;

  int startServer();
  void closeServer();
  void acceptConnection(int &new_socket);
  std::string buildResponse();
  void sendResponse();
};

}  // namespace cs::http

#endif  // CS_HTTP_SERVER_HH