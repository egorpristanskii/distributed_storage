find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(MyTests ${TEST_SOURCES})
target_link_libraries(MyTests PRIVATE GTest::gtest GTest::gtest_main kvstorage)

include(GoogleTest)
gtest_discover_tests(MyTests)
