#
# Static and dynamic libs have the same sources, so make a variable.
#

find_package(MPI REQUIRED)
if(MPI_FOUND)
	include_directories(SYSTEM ${MPI_Fortran_INCLUDE_PATH})
	include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})
else(MPI_FOUND)
	message(SEND_ERROR "This application cannot compile without MPI")
endif(MPI_FOUND)

#message(STATUS "MPI_ID_COMPILER :${MPI_C_COMPILER}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Debug flags
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
else()
        # Release flags
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()


#
# Source files
#
set(LIBCNTD_SOURCES
	calc.c
	eam.c
	init.c
	log.c
	memory.c
	monitor.c
	tool.c
	topology.c
	wrapper_pmpi.c
	wrapper_pmpi_fortran.c
)

#message(STATUS "${CMAKE_INSTALL_PREFIX}")

#
# Add dynamic library
#
add_library(cntd SHARED ${LIBCNTD_SOURCES})
target_link_libraries(cntd ${MPI_Fortran_LIBRARIES} ${MPI_C_LIBRARIES})

#
# Add static library with same base name as the dynamic lib
#
add_library(cntd-static STATIC ${LIBCNTD_SOURCES})
target_link_libraries(cntd-static ${MPI_Fortran_LIBRARIES} ${MPI_C_LIBRARIES})
set_target_properties(cntd-static PROPERTIES OUTPUT_NAME "cntd")

#
# Install target should install the two library targets above.
#
install(TARGETS cntd cntd-static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib EXPORT libcntd-libs)

#
# Headers are in ../include
#
#include_directories(${PROJECT_SOURCE_DIR}/include)
