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
	hwp.c)

# Add dynamic library
add_library(cntd SHARED ${SOURCES})
target_link_libraries(cntd 
	PRIVATE
		MPI::MPI_C
		cntd_hwloc)

set(CMAKE_C_FLAGS "-g")

# Define target architecture
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	execute_process(
		COMMAND bash -c "lscpu | grep 'Vendor ID:' | cut -d: -f2 | tr -d ' ' | tr -d '\n'"
		OUTPUT_VARIABLE ARCH)
	if("${ARCH}" STREQUAL "GenuineIntel")
		target_compile_definitions(cntd
			PRIVATE "INTEL")
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
		PRIVATE "THUNDERX2")	
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

if(CNTD_DISABLE_ACCESSORY_MPI)
	target_compile_definitions(cntd
		PRIVATE 
			"DISABLE_ACCESSORY_MPI")
endif()

if(CNTD_DISABLE_P2P_MPI)
	target_compile_definitions(cntd
		PRIVATE 
			"DISABLE_P2P_MPI")
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

# Install cntd
install(TARGETS cntd
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)