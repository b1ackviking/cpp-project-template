cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

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
enable_hardenings(options)
enable_coverage(options)

# put platform-agnostic code here
add_subdirectory(lib)

# put platform-specific code here and guard it with an option to exclude it from
# build when building unit tests (e.g. you develop an embedded firmware)
if(CMAKE_CROSSCOMPILING OR TRUE)
  add_subdirectory(src)
endif()

# put unit tests that run on your pc here
if(NOT CMAKE_CROSSCOMPILING AND BUILD_TESTING)
  add_subdirectory(test)
endif()
