# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)
# CMAKE_POLICY(SET CMP0072 NEW) Project name
project(webServer)

set(CMAKE_CACHEFILE_DIR ".build")
set(CMAKE_BUILD_FILES)

set(CMAKE_CXX_STANDARD 20) # enable C++11 standard
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/target")

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(
  webServer
  src/main.cpp
  "src/utils/logger/logger.h"
  "src/utils/logger/logger.cpp"
  "src/utils/color/color.h"
  "src/utils/color/color_helper.h"
  "src/entity/socket_server/server.h"
  "src/entity/socket_server/server.cpp"
  "src/entity/request/request.h"
  "src/entity/request/request.cpp")
