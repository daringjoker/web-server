#include "request.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "../../utils/buffer/buffer.h"
#include "../../utils/logger/logger.h"
#include "../../utils/string.h"

std::string uri_decode(std::string str) {
  return std::regex_replace(
      str, std::regex("%([0-9a-f]{2})", std::regex_constants::icase),
      [](const ::std::smatch& m) {
        const char replacement = (char)std::stoi(m.str(1), nullptr, 16);
        // Logger::Log.debug("replacing the match %%%s with '%c'",
        //                   m.str(1).c_str(), replacement);
        return std::string(1, replacement);
      });
}

Request::Request(Buffer requestBuffer) {
  unsigned long long int token_start = 0;
  unsigned long long int current_index = 0;
  unsigned long long int buf_end = requestBuffer.size();
  std::vector<std::string_view> start_line;

  bool parsing_complete = false;
  while (!parsing_complete && current_index < buf_end) {
    parsing_complete = requestBuffer[current_index] == '\n';

    if (requestBuffer[current_index] == ' ' ||
        requestBuffer[current_index] == '\n') {

      start_line.push_back(
          requestBuffer.get_view(token_start, current_index - token_start));
      token_start = current_index + 1;
    }
    current_index++;
  }

  this->method = start_line[0];
  this->URI = uri_decode(std::string(start_line[1]));
  this->http_version = start_line[2];

  std::optional<std::string_view> header_key;
  parsing_complete = false;
  while (!parsing_complete && current_index < buf_end) {
    parsing_complete =
        requestBuffer[current_index] == '\n' && !header_key.has_value();
    if (parsing_complete)
      break;

    if (!header_key.has_value() && requestBuffer[current_index] == ':') {
      header_key =
          requestBuffer.get_view(token_start, current_index - token_start);
      token_start = current_index + 1;
    }

    if (header_key.has_value() && requestBuffer[current_index] == '\n') {
      std::string_view header_value =
          requestBuffer.get_view(token_start, current_index - token_start);
      headers[header_key.value()] = header_value;
      header_key.reset();
      token_start = current_index + 1;
    }

    current_index++;
  }
  // std::cout << "HTTP VERSION : " << http_version << std::endl;
  // std::cout << "METHOD: " << method << " URI:" << URI << "\n";
  // std::cout << "\t"
  //           << "Headers:\n";
  // for (auto a : headers) {
  //   std::cout << "\t\t" << a.first << " : " << a.second << "\n";
  // }
}
