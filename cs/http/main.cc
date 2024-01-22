#include "server.hh"

int main() {
  auto server = cs::http::HttpServer("0.0.0.0", 8080);
  return server.startListening();
}
