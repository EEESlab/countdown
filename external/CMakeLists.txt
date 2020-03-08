#
# Copyright (c), University of Bologna and ETH Zurich
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
#
# Author: Daniele Cesarini, University of Bologna

function(git_submodule_update PATH)
    find_package(Git)
    # Old versions of git aren't able to run init+update
    # in one go (via 'git submodule update --init'), we need
    # to call one command for each operation:
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule init -- ${PATH}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update -- ${PATH}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endfunction(git_submodule_update)

###########################################################
# Libmsr
###########################################################
git_submodule_update(external/libmsr)
add_subdirectory(libmsr EXCLUDE_FROM_ALL)
add_library(cntd_libmsr INTERFACE)
target_link_libraries(cntd_libmsr INTERFACE msr)

set(_header_dir ${PROJECT_BINARY_DIR}/external/libmsr/include)
set(_public_headers
    ${_header_dir}/cpuid.h
    ${_header_dir}/csr_core.h
    ${_header_dir}/csr_imc.h
    ${_header_dir}/libmsr_debug.h
    ${_header_dir}/libmsr_error.h
    ${_header_dir}/master.h
    ${_header_dir}/memhdlr.h
    ${_header_dir}/msr_clocks.h
    ${_header_dir}/msr_core.h
    ${_header_dir}/msr_counters.h
    ${_header_dir}/msr_misc.h
    ${_header_dir}/msr_rapl.h
    ${_header_dir}/msr_thermal.h
    ${_header_dir}/msr_turbo.h
)

# Install libmsr
install(
    FILES ${PROJECT_BINARY_DIR}/external/libmsr/src/libmsr.so
    DESTINATION external/libmsr/lib)

install(
    FILES ${_public_headers}
    DESTINATION external/libmsr/include)