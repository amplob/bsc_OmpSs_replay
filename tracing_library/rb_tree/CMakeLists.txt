
# include header files from this folder
include_directories(${mpisstrace_SOURCE_DIR}/rb_tree)

# compile the library
add_library(rb_trees red_black_tree.c stack.c misc.c)


