CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(simple_exchange)
SET(CMAKE_CXX_STANDARD 20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(server server.cpp common.hpp json.hpp)
target_link_libraries(server PRIVATE Threads::Threads Boost::boost)

add_executable(client client.cpp common.hpp json.hpp)
target_link_libraries(client PRIVATE Threads::Threads Boost::boost)