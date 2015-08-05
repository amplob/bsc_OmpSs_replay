cmake_minimum_required(VERSION 2.8)

project (mpisstrace)

# is the testing code ON/OFF
set (library_TESTING  0)
set (PARALLEL_LIBRARY 1)
# configure a header file to pass this testing switch
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )




#link_directories (${HELLO_BINARY_DIR}/Hello)
set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicc")

# SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE Release)

# flags
add_definitions("-Wall -rdynamic -fPIC -Wl,-Bstatic")

# include header file from this folder
include_directories(${mpisstrace_SOURCE_DIR})
include_directories(${mpisstrace_SOURCE_DIR}/../common-files)
include_directories(/home/marc/tools/extrae-3.1.0-linux-x86_64-mvapich2-1.7-cuda/include)


# Recurse into the "rb_tree" subdirectory. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (rb_tree)
# add_subdirectory (tests)  

# include header files from the folder with red-black trees library
include_directories(${mpisstrace_SOURCE_DIR}/rb_tree)

# compile the library
add_library(mpissTrace SHARED  taskname_coder.c dep_calculator.c ssvalgrindcc_interface.c emit_records.c tracing_main.c)
# Link the executable to the rb_trees library.
target_link_libraries (mpissTrace rb_trees /home/marc/tools/extrae-3.1.0-linux-x86_64-mvapich2-1.7-cuda/lib/libmpitrace.so) 


install (TARGETS mpissTrace
         DESTINATION ${mpisstrace_SOURCE_DIR}/../install/lib)
install (FILES ssvalgrindcc_interface.h
         DESTINATION ${mpisstrace_SOURCE_DIR}/../install/include
         RENAME mpissTrace.h)
# install (DIRECTORY ssvalgrindcc_interface.h
#          DESTINATION ${mpisstrace_SOURCE_DIR}/../install/include
#           FILES_MATCHING PATTERN "ssvalgrindcc_interface.h")







