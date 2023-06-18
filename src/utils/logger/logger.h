#pragma once
#include <cmath>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include "../color/color.h"
enum Log_level : unsigned int {
  FATAL_ERROR = 100,
  ERROR = 200,
  WARNING = 300,
  DEBUG = 400,
  INFO = 500,
  SILLY = 600
};

class Logger {

 private:
  void log(const Log_level level, const char* format, va_list args);
  std::string prefix;

  Logger() : prefix{""} {}

 public:
  static Logger Log;

  Logger(std::string prefix) : prefix{prefix} {}
  void silly(const char* format, ...);
  void info(const char* format, ...);
  void debug(const char* format, ...);
  void warn(const char* format, ...);
  void error(const char* format, ...);
  void fatal(const char* format, ...);
};
