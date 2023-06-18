#include <netinet/in.h>
#include <functional>
#include "../../utils/logger/logger.h"
class Server {
 private:
  int server_socket = 0;
  struct sockaddr_in server_addr;
  Logger logger = Logger("Socket Server");
  std::function<int(int, int)> connection_handler = nullptr;

 public:
  Server();
  void listen_for_connections(unsigned short port,
                              unsigned int num_concurrent_connections = 100);
  void register_handler(std::function<int(int, int)> conn_handler);
  ~Server();
};
