# First check for user-specified LIBUNWIND_DIR
if(LIBUNWIND_DIR)
    set(LIBUNWIND_INCLUDE_DIR ${LIBUNWIND_DIR}/include)
    set(LIBUNWIND_LIBRARIES_DIR ${LIBUNWIND_DIR}/lib)

    find_library(LIBUNWIND_LIBRARIES
        NAMES unwind
        PATHS ${LIBUNWIND_INCLUDE_DIR}
    )

    find_path(LIBUNWIND_INCLUDE_DIRS
        NAMES libunwind.h
        PATHS ${LIBUNWIND_LIBRARIES_DIR}
    )

    if(LIBUNWIND_INCLUDE_DIRS AND LIBUNWIND_LIBRARIES)
        set(LIBUNWIND_FOUND TRUE)
        if(NOT LIBUNWIND_FIND_QUIETLY)
            message(STATUS "Found LIBUNWIND using LIBUNWIND_DIR = ${LIBUNWIND_DIR}")
            message(STATUS "  LIBUNWIND_INCLUDE_DIRS = ${LIBUNWIND_INCLUDE_DIRS}")
            message(STATUS "  LIBUNWIND_LIBRARIES = ${LIBUNWIND_LIBRARIES}")
        endif()
    else()
        set(LIBUNWIND_FOUND FALSE)
    endif()
# If LIBUNWIND_DIR not specified, then try to automatically find the UNWIND header
# and library
elseif(NOT LIBUNWIND_DIR)
    # Try to find UNWIND using pkg-config
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
         pkg_check_modules(LIBUNWIND libunwind)
         set(LIBUNWIND_LIBRARIES ${LIBUNWIND_LDFLAGS})
    endif()

    if(LIBUNWIND_FOUND)
        if(NOT LIBUNWIND_FIND_QUIETLY)
            message(STATUS "LIBUNWIND_LIBRARIES = ${LIBUNWIND_LIBRARIES}")
            message(STATUS "LIBUNWIND_INCLUDE_DIRS = ${LIBUNWIND_INCLUDE_DIRS}")
        endif()
    else()
        find_library(LIBUNWIND_LIBRARIES
            NAMES unwind
            PATHS ENV LD_LIBRARY_PATH
        )

        set(_extralibdirs)
        if(DEFINED $ENV{LIBUNWIND_HOME})
            list(APPEND _extralibdirs $ENV{LIBUNWIND_HOME}/include)
        endif()
        if(DEFINED $ENV{LIBUNWIND_ROOT})
            list(APPEND _extralibdirs $ENV{LIBUNWIND_ROOT}/include)
        endif()
        if(DEFINED $ENV{LIBUNWIND_PATH})
            list(APPEND _extralibdirs $ENV{LIBUNWIND_PATH}/include)
        endif()
        if(DEFINED $ENV{LIBUNWIND_DIR})
            list(APPEND _extralibdirs $ENV{LIBUNWIND_DIR}/include)
        endif()
        if(LIBUNWIND_LIBRARIES)
            get_filename_component(_libunwind_lib "${LIBUNWIND_LIBRARIES}" PATH)
            get_filename_component(_libunwind "${_libunwind_lib}" PATH)
            list(APPEND _extralibdirs ${_libunwind}/include)
        endif()

        find_path(LIBUNWIND_INCLUDE_DIRS
            NAMES libunwind.h
            PATHS _extralibdirs
        )

        if(LIBUNWIND_INCLUDE_DIRS AND LIBUNWIND_LIBRARIES)
            set(LIBUNWIND_FOUND TRUE)
            if(NOT LIBUNWIND_FIND_QUIETLY)
                message(STATUS "Found LIBUNWIND library using find_library()")
                message(STATUS "  LIBUNWIND_INCLUDE_DIRS = ${LIBUNWIND_INCLUDE_DIRS}")
                message(STATUS "  LIBUNWIND_LIBRARIES = ${LIBUNWIND_LIBRARIES}")
            endif()
        endif()
    endif()
endif()

# If LIBUNWIND is still not found
if(NOT LIBUNWIND_FOUND)
    if(LIBUNWIND_FIND_REQUIRED)
        message(FATAL_ERROR "LIBUNWIND not Found!")
    else()
        if(NOT LIBUNWIND_FIND_QUIETLY)
            message(WARNING "LIBUNWIND not Found!")
        endif()
    endif()
endif()