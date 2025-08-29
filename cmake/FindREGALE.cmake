if (NOT REGALE_INCLUDE_DIRS)
    find_path(
        REGALE_CORE_INCLUDE_DIR
        NAMES "regale_core.h"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "include")
    find_path(
        REGALE_INTERNALS_INCLUDE_DIR
        NAMES "regale_internals.h"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "include")
    find_path(
        REGALE_MONITOR_CLIENT_INCLUDE_DIR
        NAMES "regale_monitor.h"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "include/Monitor")
    find_path(
        REGALE_NODE_MANAGER_CLIENT_INCLUDE_DIR
        NAMES "regale_nm.h"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "include/NodeManager")

    set(REGALE_INCLUDE_DIRS
        ${REGALE_CORE_INCLUDE_DIR} ${REGALE_INTERNALS_INCLUDE_DIR} ${REGALE_MONITOR_INCLUDE_DIR} ${REGALE_NODE_MANAGER_INCLUDE_DIR})
endif()

if (NOT REGALE_LIBRARIES)
    find_library(
        REGALE_CORE_LIBRARY
        NAMES "regale_core"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "lib" "lib64")
    find_library(
        REGALE_MONITOR_CLIENT_LIBRARY
        NAMES "regale_mon_client"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "lib" "lib64")
    find_library(
        REGALE_NODE_MANAGER_CLIENT_LIBRARY
        NAMES "regale_nm_client"
        HINTS ${REGALE_ROOT}
        PATH_SUFFIXES "lib" "lib64")

    set(REGALE_LIBRARIES
        ${REGALE_CORE_LIBRARY} ${REGALE_MONITOR_CLIENT_LIBRARY} ${REGALE_NODE_MANAGER_CLIENT_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(REGALE
                                  REQUIRED_VARS REGALE_LIBRARIES REGALE_INCLUDE_DIRS
                                  REASON_FAILURE_MESSAGE "Cannot find REGALE, maybe try defining the variable -DREGALE_ROOT as the path to its installation.")

message(STATUS "regale include dir: ${REGALE_INCLUDE_DIRS}")
message(STATUS "libregale: ${REGALE_LIBRARIES}")
