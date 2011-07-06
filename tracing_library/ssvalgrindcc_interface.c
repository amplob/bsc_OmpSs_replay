

#include <stdio.h>
#include "ssvalgrindcc_interface.h"

#if library_TESTING

void start_task_valgrind(void *sp, const char * type)
{
   printf("starting task with name %s with stack pointer \n", type);
}

void end_task_valgrind(void)
{
   printf("ending task \n");
}

void start_css_valgrind (void) {
   printf("start_css_valgrind      \n");
}

void end_css_valgrind (void) {
   printf("end_css_valgrind      \n");
}

void barrier_css_valgrind (void) {
   printf("barrier_css_valgrind      \n");
}

void task_input_value_valgrind(const char *parName, int value, unsigned long element_size, unsigned long elements){
   printf("task_input_value_valgrind parName %s   value %d,  element_size %lu,   elements  %lu      \n", parName, value, element_size, elements);
}

void task_input_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   printf("task_input_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n", parName,  element_size, elements);
}
void task_output_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   printf("task_output_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n", parName, element_size, elements);
}
void task_inout_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   printf("task_inout_valgrind parName %s  ptr ,  element_size %lu,   elements  %lu      \n", parName, element_size, elements);
}


void task_unspecified_dir_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   PANIC("UNSPECIFIED DIRECTIONALITY OF PARAMETER!! parameter name %s\n", parName );
}

#endif