project (edf)

# Add defs for the compiler
add_definitions(-g -Wall -std=c++0x)

# include dirs
include_directories(${edf_SOURCE_DIR})
include_directories(${RTLib_SOURCE_DIR})



add_executable(edf edf.cpp )
							
target_link_libraries( edf ${RTLib_LIBRARIES} )
