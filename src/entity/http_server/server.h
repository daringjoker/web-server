#include "../request/request.h"
#include "../response/response.h"
#include "../socket_server/server.h"
class Http_Server {
 private:
  Socket_Server server;
  Logger logger = Logger("Http Server");

  int handle_connection(int client_socket, [[maybe_unused]] int server_socket);
  Response handle_request_statically(Request req);
  Response resolve_or_404(std::string path);

 public:
  Http_Server();
  void listen(unsigned short port);
  void use();
};
