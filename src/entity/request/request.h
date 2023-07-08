#ifndef REQUEST_H
#define REQUEST_H
#include <map>
#include <string>
#include <string_view>
#include "../../utils/buffer/buffer.h"
#include "../common/http.h"
class Request {
 private:
 public:
  std::string http_version;
  std::string method;
  std::string URI;
  std::string_view body;
  std::map<std::string_view, std::string_view> headers;
  Request(Buffer request_buffer);
};
#endif  // !REQUEST_H
