# - Try to find RTLib
# Once done, this will define
#
#  RTLib_FOUND - system has RTLib
#  RTLib_INCLUDE_DIRS - the RTLib include directories
#  RTLib_LIBRARIES - link these to use RTLib

include(LibFindMacros)

# Dependencies
libfind_package(RTLib MetaSim)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(RTLib_PKGCONF RTLib)

# Include dir
find_path(RTLib_INCLUDE_DIR
  NAMES kernel.hpp
  PATHS ${RTLib_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES rtlib
)

# Finally the library itself
find_library(RTLib_LIBRARY
  NAMES rtlib
  PATHS ${RTLib_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(RTLib_PROCESS_INCLUDES RTLib_INCLUDE_DIR MetaSim_INCLUDE_DIRS)
set(RTLib_PROCESS_LIBS RTLib_LIBRARY MetaSim_LIBRARIES)
libfind_process(RTLib)
