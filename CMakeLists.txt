CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(currency_market)
SET(CMAKE_CXX_STANDARD 20)

# finding required libs and putting them in varibale link_libs.
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(link_libs Threads::Threads Boost::context)

# connecting auxiliary scripts.
include(cmake/Helpers.cmake)

# preapring skeleton.
#add_subdirectory(skeletion)

# preparing project sources and generate main target
# and importing variables associated with certain modules.
add_subdirectory(src)