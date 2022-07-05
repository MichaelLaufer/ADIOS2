######################################################
# FindUCX
# -----------
#
# Try to find the UCX library
#
# This module defines the following variables:
#
#  UCX_FOUND - System has UCX
#  UCX_INCLUDE_DIRS - The UCX include directories
#  UCX_LIBRARIES - The libraries needed to use UCX

######################################################

# This is a bit of a wierd pattern but it allows to bypass pkg-config and
# manually specify library information
if(NOT (PC_UCX_FOUND STREQUAL "IGNORE"))
  find_package(PkgConfig)
  if(PKG_CONFIG_FOUND)
    set(_UCX_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
    if(UCX_ROOT)
      list(INSERT CMAKE_PREFIX_PATH 0 "${UCX_ROOT}")
    elseif(NOT ENV{UCX_ROOT} STREQUAL "")
      list(INSERT CMAKE_PREFIX_PATH 0 "$ENV{UCX_ROOT}")
    endif()
    set(PKG_CONFIG_USE_UCX_CMAKE_PREFIX_PATH ON)

    pkg_check_modules(PC_UCX ucx)

    set(CMAKE_PREFIX_PATH ${_UCX_CMAKE_PREFIX_PATH})
    unset(_UCX_CMAKE_PREFIX_PATH)

    if(PC_UCX_FOUND)
      if(BUILD_SHARED_LIBS)
        set(_PC_TYPE)
      else()
        set(_PC_TYPE _STATIC)
      endif()
      set(UCX_INCLUDE_DIRS ${PC_UCX${_PC_TYPE}_INCLUDE_DIRS})
      set(UCX_LIBRARIES ${PC_UCX${_PC_TYPE}_LINK_LIBRARIES})
    endif()
  endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(UCX DEFAULT_MSG UCX_LIBRARIES)

if(UCX_FOUND)
  if(NOT TARGET ucx::ucx)
    add_library(ucx::ucx INTERFACE IMPORTED)
    if(UCX_INCLUDE_DIRS)
      set_target_properties(ucx::ucx PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${UCX_INCLUDE_DIRS}"
      )
    endif()
    if(UCX_LIBRARIES)
      set_target_properties(ucx::ucx PROPERTIES
        INTERFACE_LINK_LIBRARIES      "${UCX_LIBRARIES}"
      )
    endif()
  endif()
endif()
