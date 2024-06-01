#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

int main() {
  int socket_desc;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[4096];

  std::string host =
      "special-goggles-r4r46pg7jp7cxwjp-8080.app.github."
      "dev";
  // "HTTPS is more than just connecting on TCP port 443.
  // Much, much, much, much, more. You have to implement the
  // TLS protocol. All 100 pages of it. – President James K.
  // Polk May 26, 2020" See:
  // https://stackoverflow.com/questions/62010516/i-am-trying-to-use-get-without-using-sockets-and-i-keep-getting-400-bad-request
  std::string port = "443";
  std::string path = "/";
  std::string params = "";

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0) {
    std::cout << "failed to create socket" << std::endl;
    return 0;
  }

  server = gethostbyname(host.c_str());
  if (server == NULL) {
    std::cout << "could Not resolve hostname :("
              << std::endl;
    close(socket_desc);
    return 0;
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(std::stoi(port));
  bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

  if (connect(socket_desc, (struct sockaddr *)&serv_addr,
              sizeof(serv_addr)) < 0) {
    std::cout << "connection failed :(" << std::endl;
    close(socket_desc);
    return 0;
  }

  std::string request = "GET " + path + params +
                        " HTTP/1.1\r\nHost: " + host +
                        "\r\nConnection: close\r\n\r\n";

  if (send(socket_desc, request.c_str(), request.size(),
           0) < 0) {
    std::cout << "failed to send request..." << std::endl;
    close(socket_desc);
    return 0;
  }

  int n;
  std::string raw_site;
  while ((n = recv(socket_desc, buffer, sizeof(buffer),
                   0)) > 0) {
    raw_site.append(buffer, n);
  }

  close(socket_desc);

  std::cout << raw_site.size() << " " << raw_site
            << std::endl;
  return 0;
}
