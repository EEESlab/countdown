project(libcntd C)
cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)

add_custom_target(distclean
    rm -f cmake_install.cmake
    rm -f CMakeCache.txt
    rm -f Makefile
    rm -f src/cmake_install.cmake
    rm -f src/CMakeCache.txt
    rm -f src/Makefile
    rm -rf src
    rm -rf lib/
    rm -rf CMakeFiles/
    rm -rf src/CMakeFiles/
    COMMENT "Cleaning up unwanted files" VERBATIM
)
