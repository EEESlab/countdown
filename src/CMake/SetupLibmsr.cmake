#
# Copyright (c) 2018, University of Bologna, ETH Zurich
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice, this
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

# First check find LIBMSR in user-specified $LIBMSR_DIR
# Second check find LIBMSR in the system folders
# Third check find LIBMSR in the project folder

get_filename_component(install_root "${CMAKE_INSTALL_PREFIX}" PATH)

if(LIBMSR_FORCE_EXT)
  unset(LIBMSR_INCLUDE_DIRS)
  unset(LIBMSR_LIBRARY)
else()
  find_path(
    LIBMSR_INCLUDE_DIRS
    NAMES
      cpuid.h
      memhdlr.h
      msr_core.h
      msr_rapl.h
      msr_thermal.hk
      msr_counters.h
      msr_clocks.h
      msr_misc.h
      msr_turbo.h
      csr_core.h
      csr_imc.h
      libmsr_error.h
    PATHS
      ENV CPATH
      ENV C_INCLUDE_PATH
      ENV CPLUS_INCLUDE_PATH
      ${install_root}/libmsr/include
  )

  find_library(
    LIBMSR_LIBRARY
    NAMES
      msr
    PATHS
      ENV LD_LIBRARY_PATH
      ENV LIBRARY_PATH
      ENV PATH
      ${install_root}/libmsr/lib
  )
  MESSAGE(STATUS "Looking LIBMSR using find_library()")
endif()

if(LIBMSR_INCLUDE_DIRS AND LIBMSR_LIBRARY)
  message(STATUS "LIBMSR library found using find_library()")
else()
  # If LIBMSR is still not found, then download and build LIBMSR from source
  message(STATUS "Downloading and building LIBMSR from source")
  message(STATUS " [+] Adding external project: libmsr")

  find_package(Git REQUIRED)

  include(ExternalProject)
  ExternalProject_Add(libmsr
    GIT_REPOSITORY "https://github.com/LLNL/libmsr.git"
    CMAKE_ARGS -DHWLOC_DIR=${LIBHWLOC_DIR} -DCMAKE_INSTALL_RPATH=${install_root}/libhwloc/lib -DCMAKE_INSTALL_PREFIX=${install_root}/libmsr
    INSTALL_COMMAND make install
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
  )

  set(LIBMSR_EXT TRUE)
  set(LIBMSR_INCLUDE_DIRS ${install_root}/libmsr/include)
  set(LIBMSR_LIBRARY ${install_root}/libmsr/lib/libmsr.so)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${install_root}/libmsr/lib")

  message(STATUS "ExternalDownload LIBMSR")
endif()

get_filename_component(LIBMSR_LIBRARY_PATH "${LIBMSR_LIBRARY}" PATH)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${LIBMSR_LIBRARY_PATH}")

message(STATUS "  LIBMSR_INCLUDE_DIRS = ${LIBMSR_INCLUDE_DIRS}")
message(STATUS "  LIBMSR_LIBRARY = ${LIBMSR_LIBRARY}")
