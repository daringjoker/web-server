#ifndef HTTP_HELPERS
#define HTTP_HELPERS
#include <cstdint>
#include <map>
#include <string_view>
struct Http_Version {
  unsigned long long major;
  unsigned long long minor;
};

extern std::map<unsigned short, std::string_view> reasonPhrase;
#endif  // !HTTP_HELPERS
