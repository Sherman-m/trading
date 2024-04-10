CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(currency_market)
SET(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(skeletion_common_files
  skeletion_temp/common.hpp
  skeletion_temp/json.hpp)

set(skeletion_server_files
  skeletion_temp/server.cpp
  ${skeletion_common_files})

set(skeletion_client_files
  skeletion_temp/client.cpp
  ${skeletion_common_files})

add_executable(skeletion_server ${skeletion_server_files})
target_link_libraries(skeletion_server PRIVATE Threads::Threads Boost::boost)

add_executable(skeletion_client ${skeletion_client_files})
target_link_libraries(skeletion_client PRIVATE Threads::Threads Boost::boost)