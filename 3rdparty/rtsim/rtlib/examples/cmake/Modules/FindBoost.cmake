# - Try to find MetaSim
# Once done, this will define
#
#  MetaSim_FOUND - system has MetaSim
#  MetaSim_INCLUDE_DIRS - the MetaSim include directories
#  MetaSim_LIBRARIES - link these to use MetaSim

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(Boost_PKGCONF boost)

# Include dir
find_path(Boost_INCLUDE_DIR
  NAMES atomic.hpp
  PATHS ${Boost_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES boost 
)

# Finally the library itself
find_library(Boost_LIBRARY
  NAMES boost_program_options
  PATHS ${Boost_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Boost_PROCESS_INCLUDES Boost_INCLUDE_DIR)
set(Boost_PROCESS_LIBS Boost_LIBRARY)
libfind_process(Boost)
