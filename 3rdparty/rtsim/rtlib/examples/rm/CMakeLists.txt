cmake_minimum_required (VERSION 2.6)
project (rm)

set(CMAKE_BUILD_TYPE Release)

# packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
 
# include dirs
#include_directories(${Boost_INCLUDE_DIR} ${metasim_SOURCE_DIR} ${rtlib_SOURCE_DIR}) 

# include dirs
add_subdirectory (src)