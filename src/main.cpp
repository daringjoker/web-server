#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "entity/request/request.h"
#include "entity/socket_server/server.h"
#include "utils/color/color.h"
#include "utils/logger/logger.h"

int handle_connection(int client_socket, [[maybe_unused]] int server_socket) {
  char* header = (char*)calloc(20000, sizeof(char));
  char html[] =
      "<html> <head> <title>DaringJoker</title> </head>"
      "<body><h1>This is my brand new webserver written in c</h1>"
      "<button> Be DJ</button>"
      "</body>";


  sprintf(header,
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "Content-Length: %lu \r\n"
          "Accept-Ranges: bytes \r\n"
          "Connection: close\r\n"
          "\r\n",
          strlen(html));
  long int rc = 0;
  auto msg = std::make_unique<char[]>(584);
  std::stringstream request_msg;
  do {
    rc = recv(client_socket, msg.get(), 583, 0);
    if (rc < 0)
      break;
    msg[rc] = '\0';
    request_msg << msg.get();
    Logger::Log.silly("Read %d bytes from the client", rc);
  } while (rc == 583);
  Logger::Log.info("%d Bytes Received!\n%s", request_msg.str().size(),
                   request_msg.str().c_str());
  Request req(request_msg.str());

  for (auto a : req.headers) {
    std::cout << "\t\t" << a.first << " : " << a.second << "\n";
  }
  while (send(client_socket, header, strlen(header), 0) < strlen(header))
    ;

  while (send(client_socket, html, sizeof(html), 0) < sizeof(html))
    ;
  free(header);
  return 0;
}


int main() {
  Server server = Server();
  server.register_handler(handle_connection);
  server.listen_for_connections(9999);
  return 0;
}
