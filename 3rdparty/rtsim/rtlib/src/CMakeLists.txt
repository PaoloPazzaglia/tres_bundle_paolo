# Environment-based settings.
if(WIN32)
    set(RTLIB_LIB_TYPE "STATIC")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x -D__DEBUG__")
	set(RTLIB_LIB_TYPE "SHARED")
endif()

# Create a library called "rtlib" which includes the source files.
add_library(rtlib ${RTLIB_LIB_TYPE} capacitytimer.cpp cbserver.cpp cpu.cpp 
  edfsched.cpp exeinstr.cpp fcfsresmanager.cpp feedback.cpp feedbacktest.cpp 
  fifosched.cpp fpsched.cpp grubserver.cpp interrupt.cpp jtrace.cpp 
  kernel.cpp kernevt.cpp load.cpp mrtkernel.cpp piresman.cpp pollingserver.cpp 
  reginstr.cpp regsched.cpp regtask.cpp resmanager.cpp resource.cpp 
  rmsched.cpp rrsched.cpp schedinstr.cpp schedpoints.cpp schedrta.cpp 
  scheduler.cpp server.cpp sparepot.cpp sporadicserver.cpp supercbs.cpp 
  task.cpp taskevt.cpp texttrace.cpp threinstr.cpp timer.cpp traceevent.cpp 
  waitinstr.cpp instr.cpp)

# Indicate that rtlib need metasim library.
target_link_libraries( rtlib  ${metasim_LIBRARIES} )

if(RTSIM_STANDALONE)
    # Export.
    export(TARGETS rtlib FILE "./rtlibConfig.cmake")
    export(PACKAGE rtlib)
endif()
