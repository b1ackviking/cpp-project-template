project(example)

add_subdirectory(lib)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                                              warnings options math)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
