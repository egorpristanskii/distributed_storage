cmake_minimum_required(VERSION 3.15)
project(kvstorage LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

function(enable_coverage target_name)
    if(CODE_COVERAGE)
        message(STATUS "Building with code coverage flags")
        target_compile_options(${target_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping -O0 -g)
        target_link_options(${target_name} PRIVATE -fprofile-instr-generate)
    endif()
endfunction()


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

add_library(
    app
    STATIC
     src/app.cpp
     src/config.cpp
     src/launcher.cpp
    )
target_include_directories(app PUBLIC include)
target_link_libraries(
    app
    PUBLIC
     kvstorage
     network
)
add_dependencies(app kvstorage network)

add_executable(kvstorage_service main.cpp)
add_dependencies(kvstorage_service app)
target_link_libraries(
    kvstorage_service
    PRIVATE
     app
)

enable_coverage(kvstorage)
enable_coverage(network)
enable_coverage(app)
enable_coverage(kvstorage_service)


if (ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()


install(TARGETS kvstorage_service DESTINATION /usr/local/bin)

# CPack config for .deb package
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "kvstorage")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "pristanskiyee@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Egor Pristanskii")

include(CPack)
