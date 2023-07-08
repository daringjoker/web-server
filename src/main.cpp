#include <string.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "entity/http_server/server.h"
#include "entity/request/request.h"
#include "utils/buffer/buffer.h"
#include "utils/color/color.h"
#include "utils/logger/logger.h"

int handle_connection(int client_socket, [[maybe_unused]] int server_socket) {
  char* header = (char*)calloc(20000, sizeof(char));
  char html[] =
      "<html> <head> <title>DaringJoker</title> </head>"
      "<body><h1>This is my brand new webserver written in c</h1>"
      "<div id='canvas'></div>"
      "<button "
      "onclick='fetch(`http://localhost:9999/"
      "`).then(data=>data.text()).then(data=>{"
      "const div=document.querySelector(`#canvas`);"
      "div.innerHTML= div.innerHTML+data;"
      "console.log(data,div.innerHTML);"
      "});'>"
      "Be DJ</button>"
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
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
  Buffer buf(500);
  do {
    rc = recv(client_socket, buf.get_ptr(), buf.ensured_rem(), 0);
    if (rc < 0) {
      Logger::Log.warn("Recv Failed [%d] :%s", errno, strerror(errno));
      buf.advance(0);
      break;
    }
    Logger::Log.silly("Read %d bytes from the client[ buffer size : %llu ]", rc,
                      buf.rem());
    buf.advance(rc);
  } while (buf.isFull());
  Request req(buf);
  Logger::Log.info("%d Bytes Received!", buf.size());

  while (send(client_socket, header, strlen(header), 0) < strlen(header))
    ;

  while (send(client_socket, html, sizeof(html), 0) < sizeof(html))
    ;
  free(header);
  return 0;
}


int main() {
  std::srand((unsigned int)time(0ll));
  Http_Server server;
  server.listen(9999);
  return 0;
}
