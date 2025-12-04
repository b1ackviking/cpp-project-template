project(math)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(
  ${PROJECT_NAME}
  INTERFACE FILE_SET HEADERS BASE_DIRS include FILES include/math/math.hpp
)
