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