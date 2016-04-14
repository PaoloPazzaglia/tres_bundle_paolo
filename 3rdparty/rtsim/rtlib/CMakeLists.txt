cmake_minimum_required (VERSION 2.6)
project (rtlib)

# find_package(metasim) is managed by the parent project "rtsim"
# NOP.

# Include dirs.
include_directories(${metasim_INCLUDE_DIRS})
include_directories(src)

# Add metasim libs to the search path
link_directories(${LINK_DIRECTORIES} ${metasim_LINK_DIRECTORIES})

# Add code
add_subdirectory (src)
