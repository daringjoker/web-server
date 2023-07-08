#include "./server.h"
#include <sys/socket.h>
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include "../../utils/buffer/buffer.h"
#include "../common/system.h"
#include "../request/request.h"
namespace fs = std::filesystem;

int handle_connection(int client_socket, [[maybe_unused]] int server_socket);

Http_Server::Http_Server() {
  server.register_handler([this](int client_socket, int server_socket) {
    return this->handle_connection(client_socket, server_socket);
  });
}

void Http_Server::listen(unsigned short port) {
  logger.info("Http server listening at http://localhost:%d/", port);
  server.listen_for_connections(port);
}

int Http_Server::handle_connection(int client_socket,
                                   [[maybe_unused]] int server_socket) {
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
  Buffer buf(600);
  do {
    long int rc = 0;
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
  Logger::Log.info("%d Bytes Received!", buf.size());

  Request req(buf);
  Response res = handle_request_statically(req);
  auto responseStr = res.generateOutput();
  send(client_socket, responseStr.c_str(), responseStr.size(), 0);
  logger.info("Sent response %d bytes long", responseStr.length());
  return 0;
}
long GetFileSize(std::string filename) {
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}


Response Http_Server::resolve_or_404(std::string path) {
  logger.info("Serving the '%s' file statically!", path.c_str());
  try {
    std::ifstream infile(path);
    if (!infile.is_open()) {
      throw 404;
    }
    const unsigned long long size = GetFileSize(path);
    char* buf = (char*)malloc(size + 50);
    memset(buf, 10, size + 50);
    infile.read(buf, size + 10);
    Response res;
    std::string type =
        exec((std::string("xdg-mime query filetype \"") + path + "\"").c_str());
    res.setHeader("content-type", type);
    res.send(buf, size, 200);
    return res;
  } catch (...) {
    Response res;
    res.send("NOT FOUND", 404);
    return res;
  }
}
Response list_directory(std::string path) {
  std::stringstream directoryListing;
  directoryListing << "<head><title> Index of " << path << "</title></head>";
  directoryListing << "<style>*{font-size:24px;font-family:monospace} "
                      "body{padding:0 10vw 0 10vw}</style>";
  directoryListing << "<a href=\"..\">..</a><br>";
  for (const auto& entry : fs::directory_iterator(path)) {
    directoryListing
        << "<a href=\""
        << fs::relative(entry.path(), fs::current_path()).filename().string()
        << (entry.is_directory() ? "/" : "") << "\" >"
        << fs::relative(entry.path(), path).filename().string()
        << (entry.is_directory() ? "/" : "") << "</a><br>\n";
  }
  Response res;
  res.send(directoryListing.str().c_str());
  return res;
}
Response Http_Server::handle_request_statically(Request req) {
  if (req.URI.ends_with("/")) {
    std::string index_path =
        std::string(".") + std::string(req.URI) + "index.html";
    if (GetFileSize(index_path) >= 0) {
      return resolve_or_404(index_path);
    }
    return list_directory(std::string(".") + std::string(req.URI));
  } else {
    return resolve_or_404(std::string(".") + std::string(req.URI));
  }
}
