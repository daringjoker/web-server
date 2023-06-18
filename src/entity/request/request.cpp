#include "request.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "../../utils/logger/logger.h"

Request::Request(std::string full_request_text) {
  auto buf_start = full_request_text.c_str();
  auto buf_end = buf_start + full_request_text.length();
  char* token_start = (char*)buf_start;
  char* current_char = token_start;
  std::vector<char*> start_line;

  bool parsing_complete = false;
  while (!parsing_complete && current_char < buf_end) {
    parsing_complete = *current_char == '\n';

    if (*current_char == ' ' || *current_char == '\n') {
      full_request_text[current_char - buf_start] = '\0';
      start_line.emplace_back(token_start);
      token_start = current_char + 1;
    }
    current_char++;
  }

  this->method = start_line[0];
  this->URI = start_line[1];
  char* headerKey = nullptr;
  parsing_complete = false;
  while (!parsing_complete && current_char < buf_end) {
    parsing_complete = *current_char == '\n' && headerKey == nullptr;
    if (parsing_complete)
      break;
    if (headerKey == nullptr && *current_char == ':') {
      full_request_text[current_char - buf_start] = '\0';
      headerKey = token_start;
      token_start = current_char + 1;
    }
    if (headerKey != nullptr && *current_char == '\n') {
      full_request_text[current_char - buf_start] = '\0';
      headers.emplace(headerKey, token_start);
      headerKey = nullptr;
      token_start = current_char + 1;
    }
    current_char++;
  }
  full_request_text.erase();
  std::cout << "METHOD: " << method << " URI:" << URI << "\n";
  std::cout << "\t"
            << "Headers:\n";
  for (auto a : headers) {
    std::cout << "\t\t" << a.first << " : " << a.second << "\n";
  }
}
