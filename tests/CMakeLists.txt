find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(storageTests ${TEST_SOURCES})
target_link_libraries(storageTests PRIVATE GTest::gtest GTest::gtest_main kvstorage)

include(GoogleTest)
gtest_discover_tests(storageTests)
