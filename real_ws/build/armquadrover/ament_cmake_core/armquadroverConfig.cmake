# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_armquadrover_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED armquadrover_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(armquadrover_FOUND FALSE)
  elseif(NOT armquadrover_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(armquadrover_FOUND FALSE)
  endif()
  return()
endif()
set(_armquadrover_CONFIG_INCLUDED TRUE)

# output package information
if(NOT armquadrover_FIND_QUIETLY)
  message(STATUS "Found armquadrover: 0.0.0 (${armquadrover_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'armquadrover' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${armquadrover_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(armquadrover_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${armquadrover_DIR}/${_extra}")
endforeach()
