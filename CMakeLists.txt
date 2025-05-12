cmake_minimum_required(VERSION 3.15)
project(kvstorage LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
# set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")

# set(CMAKE_CXX_CLANG_TIDY "clang-tidy-19;--config-file=/home/devel/workspace/.clang-tidy")

# Include Conan-generated files
include(${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake)

find_package(nlohmann_json REQUIRED)
find_package(asio REQUIRED)

add_library(
    kvstorage
    STATIC
     src/storage/storage.cpp
     src/storage/wal_logger.cpp
     src/storage/storage_router_adapter.cpp
    )
target_include_directories(kvstorage PUBLIC include)
target_link_libraries(
    kvstorage
    PUBLIC
     network
     nlohmann_json::nlohmann_json 
)
add_dependencies(kvstorage network)

add_library(
    network
    STATIC
     src/network/request.cpp
     src/network/response.cpp
     src/network/session.cpp
    )
target_include_directories(network PUBLIC include)
target_link_libraries(
    network
    PUBLIC
     asio::asio
     nlohmann_json::nlohmann_json 
)


# Find Boost Asio (header-only, no need to link)

add_executable(main main.cpp)
add_dependencies(main kvstorage)
target_link_libraries(
    main
    PRIVATE
     kvstorage 
     network
)

# add_executable(http_server http_server.cpp)
# target_link_libraries(http_server PRIVATE asio::asio)

if (ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
