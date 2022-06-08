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
# MOSQUITTO Options
###########################################################
option(CNTD_ENABLE_MOSQUITTO
	"Enable MQTT message passing" OFF)

###########################################################
# HWP/CPUFREQ Options
###########################################################
option(CNTD_HWP_AUTO_DISCOVER "Find automagically if hwp-states are available" ON)
option(CNTD_HWP_DISCOVERED "Manual set if hwp-states are available" OFF)
option(CNTD_USE_CPUFREQ "Manual set if use \"cpufreq\" interface" ON)

###########################################################
# PERF Options
###########################################################
if(DEFINED ENV{CNTD_MAX_NUM_CUSTOM_PERF})
	add_compile_definitions(CNTD_MAX_NUM_CUSTOM_PERF=$ENV{CNTD_MAX_NUM_CUSTOM_PERF})
endif()

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

    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        string(REGEX REPLACE ".*vendor_id[ \t]*:[ \t]+([a-zA-Z0-9_-]+).*" "\\1"
               vendor_id "${cpu_info}")

        if(vendor_id STREQUAL "GenuineIntel")
            if(CNTD_HWP_AUTO_DISCOVER)
                string(FIND "${cpu_info}" "hwp" match_hwp)

                if(${match_hwp} EQUAL -1)
                    set(hwp_avail FALSE)
                else()
                    set(hwp_avail TRUE)
                endif()
            else()
                set(hwp_avail FALSE)
                if(CNTD_HWP_DISCOVERED)
                    set(hwp_avail TRUE)
                endif()
            endif()
		else()
			set(hwp_avail FALSE)
        endif()
    else()
		set(hwp_avail FALSE)
    endif()
endif()

if(CNTD_USE_CPUFREQ)
    add_compile_definitions("CPUFREQ")
    message(STATUS "Using \"cpufreq\" interface as default one")
    message(STATUS "HWP-States will be set \"not available\" by default")
	set(hwp_avail FALSE)
endif()

if(hwp_avail)
   add_compile_definitions(HWP_AVAIL)
   message(STATUS "HWP-States available")
else()
   message(STATUS "HWP-States not available")
endif()

###########################################################
# DISCOVERING GOVERNOR
###########################################################
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(READ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" gov_info)

	if(DEFINED ENV{CNTD_GOV})
		set(gov_info $ENV{CNTD_GOV})
	endif()

	if(gov_info STREQUAL "userspace")
		add_compile_definitions(USERSPACE_GOV)
	endif()

	message(STATUS "Using \"governor\" ${gov_info}")
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

	include_directories(${MOSQUITTO_INCLUDE_DIR})

	link_libraries(${MOSQUITTO_LIBRARY})

	add_definitions(-DMOSQUITTO_ENABLED)
endif()

###########################################################
# Get compile  definitions
###########################################################
if(DEFINED ENV{CNTD_GET_COMPILE_DEFINITIONS})
	get_directory_property(DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
	foreach(d ${DirDefs})
		message(STATUS "Found compile definition: " ${d})
	endforeach()
	message(STATUS "All compile definitions: " ${DirDefs})
endif()

###########################################################
# Add directories
###########################################################
add_subdirectory(src)
