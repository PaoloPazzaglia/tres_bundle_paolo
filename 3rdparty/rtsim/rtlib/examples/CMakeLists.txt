cmake_minimum_required (VERSION 2.6)
project (edf)

# packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(RTLib REQUIRED)

# include dirs
include_directories(${RTLib_INCLUDE_DIR} ${MetaSim_INCLUDE_DIR})

# include dirs
add_subdirectory (edf)
