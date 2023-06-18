#include <map>
#include <string>
#include "../common/http.h"

class Request {
 private:
  Http_Version http_version;
  std::string method;
  std::string URI;
  std::string body;

 public:
  std::map<std::string, std::string> headers;
  Request(std::string full_request_text);
};
