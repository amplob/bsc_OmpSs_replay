
# include header files from this folder
include_directories(${mpisstrace_SOURCE_DIR}/tests)

# include header file from this folder
include_directories(${mpisstrace_SOURCE_DIR})

# Make sure the linker can find the Hello library once it is built.
link_directories (${mpisstrace_SOURCE_DIR}) 


# create executable test_taskname_coder
add_executable (test_taskname_coder test_taskname_coder.c)
# Link the executable to the rb_tree library.
target_link_libraries (test_taskname_coder mpissTrace) 

# create executable test_taskname_coder
add_executable (test_dep_calculator test_dep_calculator.c)
# Link the executable to the rb_tree library.
target_link_libraries (test_dep_calculator mpissTrace)



add_library(libmpitrace STATIC IMPORTED)
set_property(TARGET libmpitrace PROPERTY
                IMPORTED_LOCATION /home/vladimir/work/mpitrace-install/lib/libmpitrace.a)

# create executable test_taskname_coder
add_executable (mpi_probe mpi_probe.c)

# mpitrace library linking
include_directories(/gpfs/apps/CEPBATOOLS/extrae/latest/mpich/64/include)
# target_link_libraries(mpi_probe /home/vladimir/work/mpitrace-install/lib/libmpitrace.a)
# Link the executable to the rb_tree library.
target_link_libraries (mpi_probe mpissTrace /gpfs/apps/CEPBATOOLS/extrae/latest/mpich/64/lib/libmpitrace.so)  



# add_test(regtest_dep_calculator test_dep_calculator)
# add_test(regtest_taskname_coder test_taskname_coder)
