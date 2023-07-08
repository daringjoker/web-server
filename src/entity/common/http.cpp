#include "./http.h"
std::map<unsigned short, std::string_view> reasonPhrase = {{200, "OK"},
                                                           {404, "NOT FOUND"}};
