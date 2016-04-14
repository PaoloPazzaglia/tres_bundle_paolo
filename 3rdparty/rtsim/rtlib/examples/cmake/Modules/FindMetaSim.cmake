# - Try to find MetaSim
# Once done, this will define
#
#  MetaSim_FOUND - system has MetaSim
#  MetaSim_INCLUDE_DIRS - the MetaSim include directories
#  MetaSim_LIBRARIES - link these to use MetaSim

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(MetaSim_PKGCONF metasim)

# Include dir
find_path(MetaSim_INCLUDE_DIR
  NAMES metasim.hpp
  PATHS ${MetaSim_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES metasim
)

# Finally the library itself
find_library(MetaSim_LIBRARY
  NAMES metasim
  PATHS ${MetaSim_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(MetaSim_PROCESS_INCLUDES MetaSim_INCLUDE_DIR)
set(MetaSim_PROCESS_LIBS MetaSim_LIBRARY)
libfind_process(MetaSim)
