
#include "global.h"
#include <stdio.h>
#include "ssvalgrindcc_interface.h"
#include "tracing_main.h"

// #include "extrae_user_events.h"


// #include "nanos.h"
// #include "basethread.hpp"
// #include "debug.hpp"
// #include "system.hpp"
// #include "workdescriptor.hpp"
// #include "smpdd.hpp"
// #include "gpudd.hpp"
// #include "plugin.hpp"
// #include "instrumentation.hpp"
// #include "instrumentationmodule_decl.hpp"
// 

// int id = 0;
// void replay_start_task() {
//   printf("create and run wd\n");
//   Extrae_event( 1000, 1 + id);
//   ++id;
// }
// 
// void replay_end_task() {
//   printf("end task\n");
//   Extrae_event( 1000, 0);
// }

unsigned char css_initialized = 0;

void replay_start_task() {
  if (!css_initialized) {
    event_start_css();
    css_initialized = 1;
  }
  TEST_PROGRESS("starting task with name fake_name\n");
//   printf("create and run wd\n");
  event_start_task("fake_name");
}

void replay_end_task() {
//   printf("end task\n");
  TEST_PROGRESS("ending task \n");
  event_end_task();
}

// void start_task_valgrind(void *sp, const char * taskname)
// {
//    TEST_PROGRESS("starting task with name %s with stack pointer \n", taskname);
//    event_start_task (taskname);
// }
// 
// void end_task_valgrind(void)
// {
//    TEST_PROGRESS("ending task \n");
//    event_end_task();
// }

void start_css_valgrind (void) {
   TEST_PROGRESS("start_css_valgrind      \n");
   event_start_css();
}

void end_css_valgrind (void) {
   TEST_PROGRESS("end_css_valgrind      \n");
   event_end_css();
}


// void evaluate_barrier(nanos_const_wd_definition_t *const_data_ext, nanos_wd_dyn_props_t *dyn_props, 
// 		      size_t data_size, void * data, size_t num_data_accesses, nanos_data_access_t *data_accesses,
// 		      nanos_copy_data_t *copies, nanos_region_dimension_internal_t *dimensions, nanos_translate_args_t translate_args,
// 		      unsigned char & barrier_needed
// )
// {
//    barrier_needed = 0;  
// }


void barrier_css_valgrind (void) {
   TEST_PROGRESS("barrier_css_valgrind      \n");
   event_barrier();
}

void wait_on_valgrind(void *ptr) {
   TEST_PROGRESS("wait_on_valgrind      \n");
   event_wait_on(ptr);
}

void task_input_value_valgrind(const char *parName, int value, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_input_value_valgrind parName %s   value %d,  element_size %lu,   elements  %lu      \n",
                 parName, value, element_size, elements);
   /* do nothing on this one */
}

void task_input_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   TEST_PROGRESS("task_input_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n",
                 parName,  element_size, elements);
   event_input_parameter(ptr);
}
void task_output_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_output_valgrind parName %s    ptr ,  element_size %lu,   elements  %lu      \n",
                 parName, element_size, elements);
   event_output_parameter(ptr);
}
void task_inout_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("task_inout_valgrind parName %s  ptr ,  element_size %lu,   elements  %lu      \n",
                 parName, element_size, elements);
   event_inout_parameter(ptr);   
}


void task_unspecified_dir_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   PANIC("UNSPECIFIED DIRECTIONALITY OF PARAMETER!! parameter name %s\n", parName );
   /* ABORT IS ENOUGH */
}

void start_new_phase_valgrind(void) {
   TEST_PROGRESS("new_phase()      \n");
   event_new_phase();   
}
