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

# First check find LIBHWLOC in user-specified $LIBHWLOC_DIR
# Second check find LIBHWLOC in the system folders
# Third check find LIBHWLOC in the project folder

get_filename_component(install_root "${CMAKE_INSTALL_PREFIX}" PATH)

if(LIBHWLOC_FORCE_EXT)
  unset(LIBHWLOC_INCLUDE_DIRS)
  unset(LIBHWLOC_LIBRARY)
else()
  find_path(
    LIBHWLOC_INCLUDE_DIRS
    NAMES
      hwloc.h
    PATHS
      ENV CPATH
      ENV C_INCLUDE_PATH
      ENV CPLUS_INCLUDE_PATH
      ${install_root}/libhwloc/include
  )

  find_library(
    LIBHWLOC_LIBRARY
    NAMES
      hwloc
    PATHS
      ENV LD_LIBRARY_PATH
      ENV LIBRARY_PATH
      ENV PATH
      ${install_root}/libhwloc/lib
  )
  MESSAGE(STATUS "Looking LIBHWLOC using find_library()")
endif()

if(LIBHWLOC_INCLUDE_DIRS AND LIBHWLOC_LIBRARY)
  message(STATUS "LIBHWLOC library found using find_library()")
else()
  # If LIBHWLOC is still not found, then download and build LIBHWLOC from source
  message(STATUS "Downloading and building LIBHWLOC from source")
  message(STATUS " [+] Adding external project: LIBHWLOC")

  find_package(Git REQUIRED)

  include(ExternalProject)
  ExternalProject_Add(libhwloc
    GIT_REPOSITORY https://github.com/open-mpi/hwloc.git
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=${install_root}/libhwloc
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
  )

  set(LIBHWLOC_EXT TRUE)
  set(LIBHWLOC_DIR ${install_root}/libhwloc)
  set(LIBHWLOC_INCLUDE_DIRS ${LIBHWLOC_DIR}/include)
  set(LIBHWLOC_LIBRARY ${LIBHWLOC_DIR}/lib/libhwloc.so)

  message(STATUS "ExternalDownload LIBHWLOC")
endif()

message(STATUS "  LIBHWLOC_INCLUDE_DIRS = ${LIBHWLOC_INCLUDE_DIRS}")
message(STATUS "  LIBHWLOC_LIBRARY = ${LIBHWLOC_LIBRARY}")
