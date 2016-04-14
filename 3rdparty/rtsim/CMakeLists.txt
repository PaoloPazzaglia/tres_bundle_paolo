cmake_minimum_required (VERSION 2.6)
project (rtsim)

set(RTSIM_STANDALONE ON CACHE STRING "Build RTSim as stand-alone (ie, NOT included in a 3rd-party project")

if(RTSIM_STANDALONE)
    # find project's modules
    # (otherwise deps are resolved by the parent (top-level) project)
    find_package(metasim REQUIRED
                         HINTS "${CMAKE_CURRENT_LIST_DIR}/cmake") # metasim_INCLUDE_DIR,
                                                                  # metasim_LIBRARIES,
                                                                  # metasim_LINK_DIRECTORIES
    find_package(rtlib   REQUIRED
                         HINTS "${CMAKE_CURRENT_LIST_DIR}/cmake") # rtlib_INCLUDE_DIR,
                                                                  # rtlib_LIBRARIES,
                                                                  # rtlib_LINK_DIRECTORIES
endif()

# add modules' source code
add_subdirectory (metasim)
add_subdirectory (rtlib)
