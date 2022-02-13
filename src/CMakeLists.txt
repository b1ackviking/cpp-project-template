project(example)

add_subdirectory(lib)

find_package(fmt CONFIG REQUIRED)
add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                                              warnings options math fmt::fmt)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
