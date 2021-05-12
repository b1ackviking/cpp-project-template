cmake_minimum_required(VERSION 3.20)

project(
  cpp-project-template
  VERSION 0.0.1
  DESCRIPTION "Tool configuration template"
  HOMEPAGE_URL https://github.com/b1ackviking/cpp-project-template
  LANGUAGES CXX)

include(cmake/defaults.cmake)
include(cmake/cache.cmake)
include(cmake/static-analyzers.cmake)
# TODO: add sanitizers support

# TODO: add warnings and options
add_library(warnings INTERFACE)
add_library(options INTERFACE)
target_compile_features(options INTERFACE cxx_std_20)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE warnings options)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
