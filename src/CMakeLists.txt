#
# Copyright (c), CINECA, UNIBO, and ETH Zurich
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#			* Redistributions of source code must retain the above copyright notice, this
#				list of conditions and the following disclaimer.
#
#			* Redistributions in binary form must reproduce the above copyright notice,
#				this list of conditions and the following disclaimer in the documentation
#				and/or other materials provided with the distribution.
#
#			* Neither the name of the copyright holder nor the names of its
#				contributors may be used to endorse or promote products derived from
#				this software without specific prior written permission.
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

# Source files
set(SOURCES
	arch.c
	cpufreq.c
	init.c
	eam_slack.c
	pm.c
	eam.c
	report.c
	sampling.c
	tool.c
	timer.c
	wrapper_pmpi_c_cpp.c
	wrapper_pmpi_fortran.c
	hwp.c
	#${CNTD_MERIC_DIR}/src/extlib/extlib.cpp
	#${CNTD_MERIC_DIR}/cntd_meric.cpp
	# ${CNTD_MERIC_DIR}/Main.c
	)

# Add dynamic library
add_library(cntd SHARED ${SOURCES})

if(CNTD_ENABLE_MERIC)
add_dependencies(cntd build_meric)

# Specify the installation directory for MERIC
set(MERIC_INSTALL_DIR ${CMAKE_BINARY_DIR}/meric)


# Include MERIC headers
target_include_directories(cntd PRIVATE ${MERIC_INSTALL_DIR}/include)
link_directories(cntd PRIVATE ${MERIC_INSTALL_DIR}/lib)

# Link MERIC static library with COUNTDOWN
target_link_libraries(cntd PRIVATE ${MERIC_INSTALL_DIR}/lib/libmeric_ext.a)
target_link_libraries(cntd PRIVATE stdc++)
endif()

if(CNTD_ENABLE_MOSQUITTO)
	target_link_libraries(cntd
						  PRIVATE
						  MPI::MPI_C
						  cntd_hwloc
						  cntd_mosquitto)

	target_compile_definitions(cntd
							   PRIVATE
							   "MOSQUITTO_ENABLED")
elseif(CNTD_ENABLE_REGALE)
	target_link_libraries(cntd
						  PRIVATE
						  MPI::MPI_C
						  cntd_hwloc
						  cntd_regale)

	target_compile_definitions(cntd
							   PRIVATE
							   "REGALE_ENABLED")

else()
	target_link_libraries(cntd
						  PRIVATE
						  MPI::MPI_C
						  cntd_hwloc)
endif()

set(CMAKE_C_FLAGS "-g")

# Define target architecture
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	execute_process(
		COMMAND bash -c "lscpu | grep 'Vendor ID:' | cut -d: -f2 | tr -d ' ' | tr -d '\n'"
		OUTPUT_VARIABLE ARCH)
	if("${ARCH}" STREQUAL "GenuineIntel")
		if(CNTD_ENABLE_MERIC)
		target_compile_definitions(cntd
			PRIVATE "CNTD_MERIC")
		else()
            target_compile_definitions(cntd PRIVATE "INTEL")
		endif()
	elseif("${ARCH}" STREQUAL "AuthenticAMD")
		target_compile_definitions(cntd
			PRIVATE "AMD")
	else()
		message(FATAL_ERROR "It was not possible to detect the x86_64 vendor ID (INTEL|AMD)")
	endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64le")
	target_compile_definitions(cntd
		PRIVATE "POWER9")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")

	target_compile_definitions(cntd
		PRIVATE "CNTD_MERIC")	
else()
	message(FATAL_ERROR "The architecture ${CMAKE_SYSTEM_PROCESSOR} is not supported!")
endif()

# Build options
if(CNTD_ENABLE_CUDA)
	target_compile_definitions(cntd
							   PRIVATE
							   "NVIDIA_GPU")
	target_link_libraries(cntd
						  PRIVATE
						  CUDA::nvml)
endif()

if(CNTD_ENABLE_ACCESSORY_MPI)
	target_compile_definitions(cntd
							   PRIVATE
							   "ENABLE_ACCESSORY_MPI")
endif()

if(CNTD_DISABLE_ALL_MPI_EXCEPT_INI_FIN)
	target_compile_definitions(cntd
							   PRIVATE
							   "DISABLE_ALL_MPI_EXCEPT_INI_FIN")
endif()

if(CNTD_ENABLE_P2P_MPI)
	target_compile_definitions(cntd
							   PRIVATE
							   "ENABLE_P2P_MPI")
endif()

if(CNTD_ENABLE_COLLECTIVE_MPI)
	target_compile_definitions(cntd
							   PRIVATE
							   "ENABLE_COLLECTIVE_MPI")
endif()

if(CNTD_DISABLE_PROFILING_MPI)
	target_compile_definitions(cntd
							   PRIVATE
							   "DISABLE_PROFILING_MPI")
endif()

if(CNTD_ENABLE_DEBUG_MPI)
	target_compile_definitions(cntd
							   PRIVATE
							   "DEBUG_MPI")
endif()

###########################################################
# HWP/CPUfreq Options
###########################################################
if(CNTD_USE_CPUFREQ)
	target_compile_definitions(cntd
							   PRIVATE
							   "CPUFREQ")
endif()

if(CNTD_HWP_AVAIL)
	target_compile_definitions(cntd
							   PRIVATE
							   "HWP_AVAIL")
endif()

###########################################################
# PERF Options
###########################################################
if(DEFINED ENV{CNTD_MAX_NUM_CUSTOM_PERF})
	target_compile_definitions(cntd
							   PRIVATE
							   CNTD_MAX_NUM_CUSTOM_PERF=$ENV{CNTD_MAX_NUM_CUSTOM_PERF})
endif()
if(DEFINED ENV{CNTD_MAX_NUM_MEM_CHANNELS_PER_SOCKET})
	target_compile_definitions(cntd
							   PRIVATE
							   CNTD_MAX_NUM_MEM_CHANNELS_PER_SOCKET=$ENV{CNTD_MAX_NUM_MEM_CHANNELS_PER_SOCKET})
endif()

###########################################################
# REGALE Options
###########################################################
if(CNTD_ENABLE_REGALE)
    if(DEFINED ENV{CNTD_REGALE_FILE_TYPES})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_FILE_TYPES=$ENV{CNTD_REGALE_FILE_TYPES})
    endif()
    if(DEFINED ENV{CNTD_REGALE_FILE_PROFILES})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_FILE_PROFILES=$ENV{CNTD_REGALE_FILE_PROFILES})
    endif()
    if(DEFINED ENV{CNTD_REGALE_TOPIC})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_TOPIC=$ENV{CNTD_REGALE_TOPIC})
    endif()
    if(DEFINED ENV{CNTD_REGALE_MONITOR_PARTITION})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_MONITOR_PARTITION=$ENV{CNTD_REGALE_MONITOR_PARTITION})
    endif()
    if(DEFINED ENV{CNTD_REGALE_NODE_MANAGER_PARTITION})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_NODE_MANAGER_PARTITION=$ENV{CNTD_REGALE_NODE_MANAGER_PARTITION})
    endif()
    if(DEFINED ENV{CNTD_REGALE_TYPE})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_TYPE=$ENV{CNTD_REGALE_TYPE})
    endif()
    if(DEFINED ENV{CNTD_REGALE_TRANSPORT})
    	target_compile_definitions(cntd
    							   PRIVATE
    							   CNTD_REGALE_TRANSPORT=$ENV{CNTD_REGALE_TRANSPORT})
    endif()
endif()

# Install cntd
install(TARGETS cntd
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
