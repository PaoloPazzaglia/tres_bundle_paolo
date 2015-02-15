set(METASIM_CODE_PREFIX_DIR "3rdparty/rtsim/metasim/src")
set(RTLIB_CODE_PREFIX_DIR "3rdparty/rtsim/rtlib/src")
set(metasim_INCLUDE_DIRS        ${CMAKE_CURRENT_SOURCE_DIR}/${METASIM_CODE_PREFIX_DIR})
set(metasim_LIBRARIES           metasim)
set(metasim_LINK_DIRECTORIES    ${CMAKE_CURRENT_BINARY_DIR}/${METASIM_CODE_PREFIX_DIR})
set(rtlib_INCLUDE_DIRS          ${CMAKE_CURRENT_SOURCE_DIR}/${RTLIB_CODE_PREFIX_DIR})
set(rtlib_LIBRARIES             rtlib)
set(rtlib_LINK_DIRECTORIES      ${CMAKE_CURRENT_BINARY_DIR}/${RTLIB_CODE_PREFIX_DIR})
mark_as_advanced(METASIM_CODE_PREFIX_DIR)
mark_as_advanced(RTLIB_CODE_PREFIX_DIR)
