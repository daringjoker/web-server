#include <map>
#include <string_view>
#include "../../utils/buffer/buffer.h"

class Response {
 private:
  unsigned short status;
  /*
          "Content-Type: text/html\r\n"
          "Content-Length: %lu \r\n"
          "Accept-Ranges: bytes \r\n"
          "Connection: close\r\n"
  */
  std::map<std::string, std::string> headers = {{"content-type", "text/html"},
                                                {"content-length", "0"},
                                                {"accept-ranges", "none"},
                                                {"connection", "close"}};
  char* body = nullptr;
  unsigned long long int bodySize;

 public:
  void setHeader(std::string headerName, std::string value);

  int send(Buffer body, unsigned short responseCode = 200);
  int send(const char* body, unsigned short responseCode = 200);
  int send(const char* bodyContent, unsigned long long size,
           unsigned short responseCode);
  int send(void* body, unsigned long long int size,
           unsigned short responseCode = 200);
  std::string generateOutput();
};
