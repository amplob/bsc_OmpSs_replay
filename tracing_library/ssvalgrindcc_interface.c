
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

void replay_start_task(int wd_id) {
  char task_name[31];
  if (!css_initialized) {
    event_start_css();
    css_initialized = 1;
  }
  TEST_PROGRESS("starting task with name: %d\n", wd_id);
  sprintf(task_name, "%d", wd_id);
  event_start_task(&task_name[0]);
}

void replay_end_task() {
  TEST_PROGRESS("ending task \n");
  event_end_task();
}


void wait_fake(void *ptr) {
  
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
   event_barrier();
}

void wait_on_valgrind(void *ptr) {
   TEST_PROGRESS("wait_on_valgrind      \n");
   event_wait_on(ptr);
}


// void replay_input_task_value(const char *parName, int value, unsigned long element_size, unsigned long elements){
//    TEST_PROGRESS("replay_input_task_value parName %s   value %d,  element_size %lu,   elements  %lu      \n",
//                  parName, value, element_size, elements);
//    /* do nothing on this one */
// }


void replay_input_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements) {
   TEST_PROGRESS("replay_input_task parName %s    ptr ,  element_size %lu,   elements  %lu      \n", parName,  element_size, elements);
   event_input_parameter(ptr);
}


void replay_output_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("replay_output_task parName %s    ptr ,  element_size %lu,   elements  %lu      \n",
                 parName, element_size, elements);
   event_output_parameter(ptr);
}

void replay_inout_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("replay_inout_task parName %s  ptr ,  element_size %lu,   elements  %lu      \n",
                 parName, element_size, elements);
   event_inout_parameter(ptr);   
}

void replay_commutative_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements){
   TEST_PROGRESS("replay_commutative_task parName %s  ptr ,  element_size %lu,   elements  %lu      \n",
                 parName, element_size, elements);
   event_commutative_parameter(ptr);   
}

void start_new_phase_valgrind(void) {
   TEST_PROGRESS("new_phase()      \n");
   event_new_phase();   
}
