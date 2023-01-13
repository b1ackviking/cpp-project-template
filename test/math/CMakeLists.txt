project(math-test)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE math-test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE warnings options GTest::gtest
                                              GTest::gtest_main math)
gtest_discover_tests(${PROJECT_NAME} XML_OUTPUT_DIR
                     ${CMAKE_CURRENT_BINARY_DIR}/junit-reports/)
