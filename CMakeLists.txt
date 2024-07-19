cmake_minimum_required(VERSION 3.21)
project(trading CXX)
set(CMAKE_CXX_STANDARD 20)

# finding required libs
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

if (USE_SANITIZERS)
  add_compile_options(
    -g
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak)

  add_link_options(
    -g
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak)
endif()

set(TRADING_NAMESPACE "trading" CACHE STRING "C++ namespace to use")
if (NOT "${TRADING_NAMESPACE}" STREQUAL "")
    set(TRADING_NAMESPACE_BEGIN "namespace ${TRADING_NAMESPACE} {" CACHE STRING "Open C++ namespace to use")
    set(TRADING_NAMESPACE_END "}" CACHE STRING "Close C++ namespace to use")
endif()

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*pp)

add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} 
  PUBLIC
  Threads::Threads 
  Boost::timer
  Boost::context
  Boost::system)

set(TRADING_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${CMAKE_PROJECT_NAME} 
  PUBLIC
  ${TRADING_INCLUDE_DIRECTORIES})

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  "TRADING_NAMESPACE=${TRADING_NAMESPACE}"
  "TRADING_NAMESPACE_BEGIN=${TRADING_NAMESPACE_BEGIN}"
  "TRADING_NAMESPACE_END=${TRADING_NAMESPACE_END}"
)

file(GLOB_RECURSE TESTS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/*pp)
  
add_executable(${CMAKE_PROJECT_NAME}_tests ${TESTS_SOURCES}) 
target_link_libraries(${CMAKE_PROJECT_NAME}_tests PRIVATE ${CMAKE_PROJECT_NAME} gtest::gtest)


add_executable(${CMAKE_PROJECT_NAME}_exec src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_exec PRIVATE ${CMAKE_PROJECT_NAME})