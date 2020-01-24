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

# First check find LIBUNWIND in user-specified $LIBUNWIND_DIR
# Second check find LIBUNWIND in the system folders
# Third check find LIBUNWIND in the project folder

if(LIBUNWIND_FORCE_EXT)
  unset(LIBUNWIND_INCLUDE_DIRS)
  unset(LIBUNWIND_LIBRARY)
else()
  MESSAGE(STATUS "Looking LIBUNWIND using find_library()")

  find_path(
    LIBUNWIND_INCLUDE_DIRS
    NAMES
      libunwind.h
    PATHS
      ENV CPATH
      ENV C_INCLUDE_PATH
      ENV CPLUS_INCLUDE_PATH
      ${CMAKE_BINARY_DIR}/libunwind/include
  )

  find_library(
    LIBUNWIND_LIBRARY
    NAMES
      unwind
    PATHS
      ENV LD_LIBRARY_PATH
      ENV PATH
      ENV LIBRARY_PATH
      ${CMAKE_BINARY_DIR}/libunwind/lib
  )
endif()

if(LIBUNWIND_INCLUDE_DIRS AND LIBUNWIND_LIBRARY)
  message(STATUS "LIBUNWIND library found using find_library()")
else()
  # If LIBUNWIND is still not found, then download and build LIBUNWIND from source
  message(STATUS "Downloading and building LIBUNWIND from source")
  message(STATUS " [+] Adding external project: libunwind")

  find_package(Git REQUIRED)

  include(ExternalProject)
  ExternalProject_Add(libunwind
    GIT_REPOSITORY https://github.com/libunwind/libunwind.git
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/libunwind
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
  )

  set(LIBUNWIND_EXT TRUE)
  set(LIBUNWIND_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/libunwind/include)
  set(LIBUNWIND_LIBRARY ${CMAKE_BINARY_DIR}/libunwind/lib/libunwind.so)

  message(STATUS "ExternalDownload LIBUNWIND")
endif()

message(STATUS "  LIBUNWIND_INCLUDE_DIRS = ${LIBUNWIND_INCLUDE_DIRS}")
message(STATUS "  LIBUNWIND_LIBRARY = ${LIBUNWIND_LIBRARY}")
