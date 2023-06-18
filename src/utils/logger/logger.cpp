#include "logger.h"
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include "../color/color.h"

std::map<Log_level, std::string> log_level_prefex = {
    {Log_level::INFO, "🧋   Info"},        {Log_level::ERROR, "🐞  Error"},
    {Log_level::SILLY, "👅  Silly"},       {Log_level::DEBUG, "🐛  Debug"},
    {Log_level::FATAL_ERROR, "💀  Fatal"}, {Log_level::WARNING, " ❗Warn"}};

std::map<Log_level, std::string> log_level_color = {
    {Log_level::INFO, Color::green},
    {Log_level::ERROR, Color::brightRed},
    {Log_level::SILLY, Color::lightBlue},
    {Log_level::DEBUG, Color::pink},
    {Log_level::FATAL_ERROR,
     std::string(Color::white) + Color::esc::blink + Color::bg::red},
    {Log_level::WARNING, Color::yellow}};

Logger Logger::Log = Logger();

void Logger::log(const Log_level level, const char* format, va_list args) {
  const time_t now = time(nullptr);
  auto timeStamp = std::make_unique<char[]>(40);
  strftime(timeStamp.get(), 40, "%r %a %d %b %Y (%z)", localtime(&now));
  // show timestamp
  std::cout << Color::esc::bold << Color::bg::lightYellow << Color::darkGray
            << " " << timeStamp.get() << " " << Color::esc::resetAll;
  // show prefix if prefix is available
  if (prefix.length() > 0) {
    std::cout << Color::white << Color::bg::lightBlue << std::right << " "
              << prefix + " " << Color::esc::resetAll;
  }

  // show the log_level prefix
  std::cout << Color::purple << Color::bg::darkGray << " " << std::setw(28)
            << std::left
            << log_level_prefex[level] + " " + Color::esc::resetAll;
  //show the msg
  std::cout << Color::esc::bold << log_level_color[level];
  vprintf(format, args);
  std::cout << Color::esc::resetAll << "\n";
}


void Logger::silly(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::SILLY, format, args);
  va_end(args);
}
void Logger::info(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::INFO, format, args);
  va_end(args);
}
void Logger::debug(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::DEBUG, format, args);
  va_end(args);
}
void Logger::warn(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::WARNING, format, args);
  va_end(args);
}
void Logger::error(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::ERROR, format, args);
  va_end(args);
}
void Logger::fatal(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(Log_level::FATAL_ERROR, format, args);
  va_end(args);
}
