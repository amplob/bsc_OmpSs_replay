cmake_minimum_required(VERSION 2.8)

project (mpisstrace)

# is the testing code ON/OFF
set (library_TESTING 1)
# configure a header file to pass this testing switch
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )




#link_directories (${HELLO_BINARY_DIR}/Hello)
set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicc")

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

# flags
add_definitions("-Wall -rdynamic")

# include header file from this folder
include_directories(${mpisstrace_SOURCE_DIR})

# Recurse into the "rb_tree" subdirectory. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (rb_tree)
add_subdirectory (tests)  

# include header files from the folder with red-black trees library
include_directories(${mpisstrace_SOURCE_DIR}/rb_tree)

# compile the library
add_library(mpissTrace taskname_coder.c dep_calculator.c)
# Link the executable to the rb_trees library.
target_link_libraries (mpissTrace rb_trees) 







