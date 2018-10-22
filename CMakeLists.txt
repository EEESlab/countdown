#
# Copyright (c) 2018, University of Bologna, ETH Zurich
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#		* Redistributions of source code must retain the above copyright notice, this
#        list of conditions and the following disclaimer.
# 
#      * Redistributions in binary form must reproduce the above copyright notice,
#        this list of conditions and the following disclaimer in the documentation
#        and/or other materials provided with the distribution.
# 
#      * Neither the name of the copyright holder nor the names of its
#        contributors may be used to endorse or promote products derived from
#        this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# Author: Daniele Cesarini, University of Bologna
# Date: 24.08.2018

project(libcntd C)
set(libcntd_VERSION_MAJOR 2)
set(libcntd_VERSION_MINOR 0)

cmake_minimum_required(VERSION 3.0.2)

# Set Release/Debug
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find MPI
find_package(MPI REQUIRED)
if(MPI_FOUND)
    include_directories(SYSTEM ${MPI_Fortran_INCLUDE_PATH})
    include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})
else(MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif(MPI_FOUND)

# Find Libmsr
if(NOT LIBMSR)
    find_library(LIBMSR_LOCAL
        NAMES
            libmsr.so
            libmsr.a
        HINTS
            ${CMAKE_BINARY_DIR}/libmsr/install
        PATHS
            ${CMAKE_BINARY_DIR}/libmsr/install
        PATH_SUFFIXES
            "lib"
    )

    if(NOT LIBMSR_LOCAL)
    	message(STATUS "Not found Libmsr: Installing from Github repository https://github.com/LLNL/libmsr.git")
        find_package(Git REQUIRED)

        include(ExternalProject)
        ExternalProject_Add(LIBMSR
            PREFIX libmsr
            TMP_DIR libmsr/tmp
            STAMP_DIR libmsr/stamp
            SOURCE_DIR libmsr/src
            BINARY_DIR libmsr/build
            GIT_REPOSITORY "https://github.com/LLNL/libmsr.git"
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=../install
        )

        set(LIBMSR ${CMAKE_BINARY_DIR}/libmsr/install)
    else()
        get_filename_component(LIBMSR_LOCAL ${LIBMSR_LOCAL} PATH)
        get_filename_component(LIBMSR ${LIBMSR_LOCAL} PATH)
        message(STATUS "Found Libmsr: ${LIBMSR}")
    endif()
else()
    message(STATUS "Found Libmsr: ${LIBMSR}")
endif()

# Common flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

# Flags for Debug or Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
else()
    # Release flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

# Flags for different compilers
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
    # using Intel
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # using CLANG
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio
endif()

# Add directories
add_subdirectory(src)

# Add distclean to make
add_custom_target(distclean
    rm -f cmake_install.cmake
    rm -f CMakeCache.txt
    rm -f Makefile
    rm -rf bin
    rm -rf lib
    rm -rf CMakeFiles
    rm -rf libmsr
    rm -f src/cmake_install.cmake
    rm -f src/Makefile
    rm -rf src/CMakeFiles
    COMMENT "Cleaning up unwanted files" VERBATIM
)
