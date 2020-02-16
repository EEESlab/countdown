#
# Copyright (c) 2018, University of Bologna, ETH Zurich
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#		* Redistributions of source code must retain the above copyright notice, this
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
# Date: 24.08.2018

# Source files
set(SOURCES
	init.c
	memory.c
	monitor.c
	log.c
	pm.c
	eam.c
	eam_slack.c
	wrapper_pmpi_c_cpp.c
	wrapper_pmpi_fortran.c
	tool.c
	calc.c
	share.c
	arch.c
	batch.c
	barrier.c
	andante.c
	fermata.c
	stacktrace/md5.c
	stacktrace/stacktrace.c
)

# Create library
add_library(cntd SHARED ${SOURCES})

target_link_libraries(cntd 
	PRIVATE
		cntd_mpi_c
		msr:msr
		cntd_libunwind
)

#target_include_directories(cntd
#	PUBLIC
#		"${libmsr_SOURCE_DIR}/include"
#)

#if(NOT MPI_FOUND)
#    message(SEND_ERROR "This application cannot compile without MPI")
#endif()
#include(CMake/Setup3rdParty.cmake)
#
#
#
## Add header paths
#target_include_directories(cntd PRIVATE
#	${MPI_C_INCLUDE_PATH}
#	${LIBUNWIND_INCLUDE_DIRS}
#	${LIBMSR_INCLUDE_DIRS}
#)
#
#target_include_directories(cntd-static PRIVATE
#	${MPI_C_INCLUDE_PATH}
#	${LIBUNWIND_INCLUDE_DIRS}
#	${LIBMSR_INCLUDE_DIRS}
#)
#
## Add libraries
#target_link_libraries(cntd PRIVATE
#	${MPI_C_LIBRARIES}
#	${LIBUNWIND_LIBRARY}
#	${LIBMSR_LIBRARY}
#)
#
## Add compile flags
#set_target_properties(cntd PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS}")
#set_target_properties(cntd-static PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS}")
#
## Add link flags
#set_target_properties(cntd PROPERTIES LINK_FLAGS "${MPI_C_LINK_FLAGS}")
#set_target_properties(cntd-static PROPERTIES LINK_FLAGS "${MPI_C_LINK_FLAGS}")
#
## Add static library with same base name as the dynamic lib
#set_target_properties(cntd-static PROPERTIES OUTPUT_NAME "cntd")
#if(LIBHWLOC_EXT)
#	add_dependencies(libmsr libhwloc)
#endif()
#if(LIBUNWIND_EXT)
#	add_dependencies(cntd libunwind)
#	add_dependencies(cntd-static libunwind)
#endif()
#if(LIBMSR_EXT)
#	add_dependencies(cntd libmsr)
#	add_dependencies(cntd-static libmsr)
#endif()
#
## Install target should install the two library targets above.
#install(TARGETS cntd cntd-static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)