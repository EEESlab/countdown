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
option(CNTD_ENABLE_MOSQUITTO
	"Enable MQTT message passing" OFF)

###########################################################
# Build Type
###########################################################
set(default_build_type "RelWithDebInfo")

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
# DISCOVERING ARCHITECTURE
###########################################################
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      file(READ "/proc/cpuinfo" cpu_info)
      string(REGEX REPLACE ".*vendor_id[ \t]*:[ \t]+([a-zA-Z0-9_-]+).*" "\\1"
             vendor_id "${cpu_info}")
      string(REGEX REPLACE ".*cpu family[ \t]*:[ \t]+([a-zA-Z0-9_-]+).*" "\\1"
             cpu_family "${cpu_info}")
      string(REGEX REPLACE ".*model[ \t]*:[ \t]+([a-zA-Z0-9_-]+).*" "\\1"
             cpu_model "${cpu_info}")
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(vendor_id STREQUAL "GenuineIntel")
   set(hwp_avail FALSE)
   if(cpu_family EQUAL 6)
      if(cpu_model EQUAL 133) # 0x85
         set(TARGET_ARCHITECTURE "Knights Mill")
      elseif(cpu_model EQUAL 87) # 0x57
         set(TARGET_ARCHITECTURE "Knights Landing")
      elseif(cpu_model EQUAL 140 OR cpu_model EQUAL 141) # 0x8C, 0x8D
         set(TARGET_ARCHITECTURE "Tiger Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 125 OR cpu_model EQUAL 126) # 0x7D, 0x7E
         set(TARGET_ARCHITECTURE "Ice Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 165 OR cpu_model EQUAL 166) # 0xA5, 0xA6
         set(TARGET_ARCHITECTURE "Comet Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 102) # 0x66
         set(TARGET_ARCHITECTURE "Cannon Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 142 OR cpu_model EQUAL 158) # 0x8E, 0x9E
         set(TARGET_ARCHITECTURE "Kaby Lake/Coffee Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 106 OR cpu_model EQUAL 108) # 0x6A, 0x6C
         set(TARGET_ARCHITECTURE "Ice Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 85) # 0x55
         set(TARGET_ARCHITECTURE "Skylake/Cascade Lake/Cooper Lake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 78 OR cpu_model EQUAL 94) # 0x4E, 0x5E
         set(TARGET_ARCHITECTURE "Skylake")
         set(hwp_avail TRUE)
      elseif(cpu_model EQUAL 61 OR cpu_model EQUAL 71
             OR cpu_model EQUAL 79 OR cpu_model EQUAL 86) # 0x3D, 0x47,
         set(TARGET_ARCHITECTURE "Broadwell")               # 0x4F, 0x56
      elseif(cpu_model EQUAL 60 OR cpu_model EQUAL 63     # 0x3C, 0x3F,
             OR cpu_model EQUAL 69 OR cpu_model EQUAL 70) # 0x45, 0x46
         set(TARGET_ARCHITECTURE "Haswell")
      elseif(cpu_model EQUAL 58 OR cpu_model EQUAL 62) # 0x3A, 0x3E
         set(TARGET_ARCHITECTURE "Ivy Bridge")
      elseif(cpu_model EQUAL 45) # 0x2D
         set(TARGET_ARCHITECTURE "Sandy Bridge")
      endif()
   endif()
endif(vendor_id STREQUAL "GenuineIntel")

message(STATUS "Current Architecture: ${TARGET_ARCHITECTURE}")

if(hwp_avail)
   add_definitions(-DHWP_AVAIL)
   message(STATUS "HWP-States available")
else()
   message(STATUS "HWP-States not available")
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
# MOSQUITTO
###########################################################
if(CNTD_ENABLE_MOSQUITTO)
	find_package(MOSQUITTO REQUIRED)
endif()

###########################################################
# Add directories
###########################################################
add_subdirectory(src)
