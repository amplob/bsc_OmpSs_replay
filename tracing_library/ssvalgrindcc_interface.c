

#include <stdio.h>
#include "ssvalgrindcc_interface.h"
#include "tracing_main.h"

void start_task_valgrind(void *sp, const char * taskname)
{
   TEST_PROGRESS("starting task with name %s with stack pointer \n", taskname);
   event_start_task (taskname);
}

void end_task_valgrind(void)
{
   TEST_PROGRESS("ending task \n");
   event_end_task();
}

void start_css_valgrind (void) {
   TEST_PROGRESS("start_css_valgrind      \n");
   event_start_css();
}

void end_css_valgrind (void) {
   TEST_PROGRESS("end_css_valgrind      \n");
   event_end_css();
}

void barrier_css_valgrind (void) {
   TEST_PROGRESS("barrier_css_valgrind      \n");
}

void wait_on_valgrind(void *ptr) {
   TEST_PROGRESS("wait_on_valgrind      \n");
   
}

void task_input_value_valgrind(const char *parName, int value, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_input_value_valgrind parName %s   value %d,  element_size %lu,   elements  %lu      \n", parName, value, element_size, elements);
}

void task_input_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   TEST_PROGRESS("task_input_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n", parName,  element_size, elements);
}
void task_output_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_output_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n", parName, element_size, elements);
}
void task_inout_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_inout_valgrind parName %s  ptr ,  element_size %lu,   elements  %lu      \n", parName, element_size, elements);
}


void task_unspecified_dir_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   PANIC("UNSPECIFIED DIRECTIONALITY OF PARAMETER!! parameter name %s\n", parName );
}

void start_new_phase_valgrind(void) {
   TEST_PROGRESS("new_phase()      \n");
   event_new_phase();   
}
