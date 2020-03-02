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

# Install libmsr
install(
    FILES ${PROJECT_BINARY_DIR}/external/libmsr/src/libmsr.so
    DESTINATION external)