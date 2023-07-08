#include "server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <functional>
#include <memory>


Socket_Server::Socket_Server() {
  this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (this->server_socket < 0) {
    logger.fatal("Socket Creation Failed! %s", strerror(errno));
    throw errno;
  }

  logger.silly("Socket with descriptor %d created Successfully!",
               this->server_socket);
  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
  logger.silly("Socket option set to make it reuse port!");
}

void Socket_Server::listen_for_connections(
    unsigned short port, unsigned int num_concurrent_connections) {
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  int bind_res =
      bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

  if (bind_res < 0) {
    logger.fatal("Socket Could not be bound to %s", strerror(errno));
    throw errno;
  }

  logger.silly("Binding Successful! server is bound to port %d", port);
  int listen_res = listen(server_socket, num_concurrent_connections);
  if (listen_res < 0) {
    logger.fatal("Failed to listen on the socket: %s", strerror(errno));
    throw errno;
  } else {
    logger.info("Socket Server is listening! at Port %u",
                ntohs(server_addr.sin_port));
  }

  while (true) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    memset(&client_addr, 0, addr_len);
    int client_socket =
        accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket > 0) {
      auto client_ip = std::make_unique<char*>(inet_ntoa(client_addr.sin_addr));

      logger.info("New socket connection recieved as fd %d from %s:%u",
                  client_socket, *client_ip, ntohs(client_addr.sin_port));

      if (connection_handler == nullptr) {
        logger.error(
            "No handler has been assigned to handle the connection! closing "
            "the connection now!");
      } else {
        connection_handler(client_socket, server_socket);
      }

      shutdown(client_socket, SHUT_RDWR);
      close(client_socket);
    }
  }
}

void Socket_Server::register_handler(
    std::function<int(int, int)> conn_handler) {
  logger.info("Regestering a Connection Handler");
  connection_handler = conn_handler;
}

Socket_Server::~Socket_Server() {
  close(server_socket);
}
