#include "./response.h"
#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "../common/http.h"

void Response::setHeader(std::string headerName, std::string value) {
  headers[headerName] = value;
}

int Response::send(const char* bodyContent, unsigned short responseCode) {
  return send((void*)bodyContent, strlen(bodyContent), responseCode);
}
int Response::send(const char* bodyContent, unsigned long long size,
                   unsigned short responseCode) {
  return send((void*)bodyContent, size, responseCode);
}
int Response::send(Buffer bodyContent, unsigned short responseCode) {
  return send((void*)bodyContent.get_view(0, bodyContent.size()).data(),
              bodyContent.size(), responseCode);
}
int Response::send(void* bodyContent, unsigned long long int size,
                   unsigned short responseCode) {
  status = responseCode;
  this->body = (char*)malloc(size + 10);
  memcpy(this->body, bodyContent, size);
  const std::string sizeStr = std::to_string(size);
  bodySize = size;
  headers["content-length"] = sizeStr;
  return 0;
}
std::string sanitize(std::string str) {
  return std::regex_replace(str, std::regex("\\r*\\n+"), " ");
}
std::string Response::generateOutput() {
  const char* CRLF = "\r\n";
  std::stringstream resp;
  resp << "HTTP/1.1 " << status << " " << reasonPhrase[status] << CRLF;
  for (const auto& hdr : headers) {
    resp << sanitize(hdr.first) << ": " << sanitize(hdr.second) << CRLF;
  }
  resp << CRLF;
  resp.write(body, bodySize);
  return resp.str();
}
