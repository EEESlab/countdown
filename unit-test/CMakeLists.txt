#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fsanitize=leak")
add_executable(hello hello.c)
target_link_libraries(hello ${MPI_LIBRARIES})
target_include_directories(hello PRIVATE ${MPI_INCLUDE_PATH})
# find executable path
# add test to run the executable with mpirun
#add_test(NAME Unit-test COMMAND "srun" "${CMAKE_BINARY_DIR}/unit-test/hello")
add_test(NAME Unit-test COMMAND "mpirun" "${CMAKE_BINARY_DIR}/unit-test/hello")
set_property(TEST Unit-test PROPERTY ENVIRONMENT LD_PRELOAD=/leonardo/home/userinternal/amonteru/countdown/build/src/libcntd.so)
