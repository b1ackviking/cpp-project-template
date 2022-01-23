cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(
  cpp-project-template
  VERSION 0.0.1
  DESCRIPTION "Tool configuration template"
  HOMEPAGE_URL https://github.com/b1ackviking/cpp-project-template
  LANGUAGES CXX)

include(cmake/defaults.cmake)
include(FindThreads)
include(CTest)

add_library(warnings INTERFACE)
set_target_warnings(warnings)

add_library(options INTERFACE)
target_compile_features(options INTERFACE cxx_std_23)
enable_sanitizers(options)
enable_coverage(options)

add_subdirectory(src)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
