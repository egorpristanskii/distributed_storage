cmake_minimum_required(VERSION 3.15)
project(BoostAsioExample LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy-19;--config-file=/home/devel/workspace/.clang-tidy")

# Include Conan-generated files
include(${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake)

find_package(nlohmann_json REQUIRED)

add_library(stringstorage STATIC src/string_storage.cpp)
target_include_directories(stringstorage PUBLIC include)
target_link_libraries(stringstorage PRIVATE nlohmann_json::nlohmann_json)

# Find Boost Asio (header-only, no need to link)
# find_package(Boost REQUIRED COMPONENTS asio algorithm)

add_executable(main main.cpp)
add_dependencies(main stringstorage)
target_link_libraries(main PRIVATE stringstorage)
