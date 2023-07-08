#include "./color.h"
std::string rgb_bg(uint8_t r, uint8_t g, uint8_t b) {
  return std::string("\033[48;2;") + std::to_string(r) + ";" +
         std::to_string(g) + ";" + std::to_string(b) + "m";
}
