#include "../socket_server/server.h"

class Http_server {
 private:
  Server server;

 public:
  Http_server();
  void listen(int port);
};
