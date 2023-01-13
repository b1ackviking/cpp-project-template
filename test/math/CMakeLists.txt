project(math-test)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE math-test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE warnings options GTest::gtest
                                              GTest::gtest_main math)
gtest_discover_tests(${PROJECT_NAME} DISCOVERY_MODE PRE_TEST)
