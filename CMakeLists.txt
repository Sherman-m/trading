CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(currency_market)
SET(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

# Skeletion
set(skeletion_common_files
  skeletion_temp/common.hpp
  skeletion_temp/json.hpp)

set(skeletion_server_files
  skeletion_temp/server.cpp
  ${skeletion_common_files})

set(skeletion_client_files
  skeletion_temp/client.cpp
  ${skeletion_common_files})

# Util
set(util 
    src/util/tagged.hpp
    src/util/tagged_numeric.hpp)

# src
set(src
    ${util})

add_executable(skeletion_server ${skeletion_server_files})
target_link_libraries(skeletion_server PRIVATE Threads::Threads Boost::boost)

add_executable(skeletion_client ${skeletion_client_files})
target_link_libraries(skeletion_client PRIVATE Threads::Threads Boost::boost)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${src})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads Boost::boost)