project(example)

find_package(fmt CONFIG REQUIRED)
add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                                              warnings options math fmt::fmt)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME_DEPENDENCY_SET
                ${PROJECT_NAME}_runtime_dependencies)
install(RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}_runtime_dependencies)
