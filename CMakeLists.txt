# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.12)
project(webServer)

function(print header)
  string(JOIN "\n\t" arrMsg ${ARGN})
  message("${header} :\n\t${arrMsg}\n")
endfunction()

set(CMAKE_CACHEFILE_DIR "${PROJECT_SOURCE_DIR}/.build")

set(CMAKE_CXX_STANDARD 20)# enable C++20 standard
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/target")

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE all_source_cpp_files CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE all_source_h_files CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.h")

print("CPP sources: " ${all_source_cpp_files})
print("H sources: " ${all_source_h_files})

add_executable(
  webServer
  ${all_source_cpp_files}
  ${all_source_h_files}
)
