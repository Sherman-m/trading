CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(currency_market)
SET(CMAKE_CXX_STANDARD 20)

# finding required libs and putting them in varibale link_libs.
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(link_libs 
  Threads::Threads 
  Boost::timer
  Boost::context
  Boost::system)

# using santizers
set(USE_SANITIZERS ON)

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
    -fsanitize=leak
  )
endif()

# connecting auxiliary scripts.
include(cmake/Helpers.cmake)

# preapring skeleton.
#add_subdirectory(skeletion)

# preparing project sources and generate main target
# and importing variables associated with certain modules.
add_subdirectory(src)

# if CMAKE_BUILD_TYPE equal Debug, then tests will be prepared and generated test targets.
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_subdirectory(tests)
endif()