# Environment-based settings.
if(WIN32)
    set(METASIM_LIB_TYPE "STATIC")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x -D__DEBUG__")
	set(METASIM_LIB_TYPE "SHARED")
endif()

# Create a library called "metasim" which includes the source files.
add_library(metasim ${METASIM_LIB_TYPE} basestat.cpp entity.cpp genericvar.cpp
  regvar.cpp strtoken.cpp trace.cpp debugstream.cpp event.cpp
  randomvar.cpp simul.cpp tick.cpp)

if(RTSIM_STANDALONE)
    # Export.
    export(TARGETS metasim FILE "./metasimConfig.cmake")
    export(PACKAGE metasim)
endif()
