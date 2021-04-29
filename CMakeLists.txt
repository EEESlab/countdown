#
# Copyright (c), CINECA, UNIBO, and ETH Zurich
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#	   * Redistributions of source code must retain the above copyright notice, this
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

cmake_minimum_required(VERSION 3.0)

project(libcntd
    VERSION 2.0
    DESCRIPTION "COUNTDOWN: A Run-time Library for Application-agnostic Energy Saving in MPI Communication Primitives"
    LANGUAGES C)

# Policy CMP0074: this policy is to use <PackageName>_ROOT variables if defined
cmake_policy(SET CMP0074 NEW)

###########################################################
# Build helpers
###########################################################
set(PROJECT_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

###########################################################
# Options
###########################################################
option(CNTD_ENABLE_CUDA
    "Enable CUDA monitoring on NVIDIA GPUs" OFF)
option(CNTD_DISABLE_PROFILING_MPI
    "Disable MPI profiling" OFF)
option(CNTD_DISABLE_ACCESSORY_MPI
    "Disable the instrumentation of all accessory MPI functions" OFF)
option(CNTD_DISABLE_P2P_MPI
    "Disable the instrumentation of all point-to-point MPI functions" OFF)
option(CNTD_ENABLE_DEBUG_MPI
    "Enable the debug prints on MPI functions" OFF)

###########################################################
# Build Type
###########################################################
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}"
        CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE
        PROPERTY
            STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

###########################################################
# MPI
###########################################################
find_package(MPI REQUIRED C)

###########################################################
# HWLOC
###########################################################
find_package(HWLOC REQUIRED)
add_library(cntd_hwloc INTERFACE)
target_link_libraries(cntd_hwloc
    INTERFACE ${HWLOC_LIBRARIES})
target_include_directories(cntd_hwloc
    INTERFACE ${HWLOC_INCLUDE_DIRS})

###########################################################
# CUDA
###########################################################
if(CNTD_ENABLE_CUDA)
    # CMake < 3.17 cannot use find_package(CUDAToolkit)
    cmake_minimum_required(VERSION 3.17)
    find_package(CUDAToolkit REQUIRED)
endif()

###########################################################
# Add directories
###########################################################
add_subdirectory(src)