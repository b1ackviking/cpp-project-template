cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(
  cpp-project-template
  VERSION 0.0.1
  DESCRIPTION "Tool configuration template"
  HOMEPAGE_URL https://github.com/b1ackviking/cpp-project-template
  LANGUAGES CXX)

include(cmake/defaults.cmake)
include(FindThreads)

add_library(warnings INTERFACE)
set_target_warnings(warnings)

add_library(options INTERFACE)
target_compile_features(options INTERFACE cxx_std_23)
enable_sanitizers(options)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE warnings options
                                              ${CMAKE_THREAD_LIBS_INIT})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)