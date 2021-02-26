#
# Copyright (c), University of Bologna and ETH Zurich
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
#
# Author: Daniele Cesarini, University of Bologna

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
	wrapper_pmpi_c_cpp.c
	wrapper_pmpi_fortran.c)

# Add dynamic library
add_library(cntd SHARED ${SOURCES})
target_link_libraries(cntd 
	PRIVATE
		MPI::MPI_C)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	target_compile_definitions(cntd
		PRIVATE 
			"INTEL")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64le")
	target_compile_definitions(cntd
		PRIVATE 
			"POWER9")
else()
	message(FATAL "The architecture ${CMAKE_SYSTEM_PROCESSOR} is not supported!")
endif()

if(CNTD_ENABLE_ALL_MPI)
	target_compile_definitions(cntd
		PRIVATE 
			"NVIDIA_GPU")
endif()

if(CNTD_ENABLE_CUDA)
	target_compile_definitions(cntd
		PRIVATE 
			"ENABLE_ALL_MPI")
	target_link_libraries(cntd 
		PRIVATE
			CUDA::nvml)
endif()

if(CNTD_DISABLE_MPI_PROFILING)
	target_compile_definitions(cntd
		PRIVATE 
			"DISABLE_MPI_PROFILING")
endif()

# Install cntd
install(TARGETS cntd
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)