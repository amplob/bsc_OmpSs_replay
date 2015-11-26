#include "emit_records.h"
#include "extrae_user_events.h"

/**************************************************************************
 *    FUNCTIONS THAT ARE INTERFACE TO tracing_main module
 **************************************************************************/

void emit_css_start(void) {
   TEST_PROGRESS("emit css start \n");
   Extrae_event(COMMON_EVENT_TYPE_CSS_START, 1);
   Extrae_event(COMMON_EVENT_TYPE_PHASEID, 0);   
}

void emit_css_finish(void) {
   TEST_PROGRESS("emit css finish \n");
   Extrae_event(COMMON_EVENT_TYPE_CSS_START, 0);
}


void emit_task_start(t_taskcode task_code, t_taskId task_number) {
   
   TEST_PROGRESS("emit task start code %d, number %d\n",
                  task_code, task_number);
   Extrae_event(COMMON_EVENT_TYPE_TASKID, task_number);
   Extrae_event(COMMON_EVENT_TYPE_TASKNAME_CODED, task_code);   
}

void emit_task_end(void) {
   
   TEST_PROGRESS("emit task end\n");
   Extrae_event(COMMON_EVENT_TYPE_TASKNAME_CODED, 0);
   Extrae_event(COMMON_EVENT_TYPE_TASKID, 0);
}

void emit_commutative_start(int mappedAddr) {
   TEST_PROGRESS("emit commutative task start with mapped address %d\n", mappedAddr);
   Extrae_event(COMMON_EVENT_TYPE_CRITICAL_BEGIN, mappedAddr);  
}

void emit_commutative_end(int mappedAddr) {
   TEST_PROGRESS("emit commutative task end with mapped address %d\n", mappedAddr);
   Extrae_event(COMMON_EVENT_TYPE_CRITICAL_END, mappedAddr);
}

void emit_concurrent_start(int mappedAddr) {
   TEST_PROGRESS("emit concurrent task start with mapped address %d\n", mappedAddr);
   Extrae_event(COMMON_EVENT_TYPE_CONCURRENT_BEGIN, mappedAddr);  
}

void emit_concurrent_end(int mappedAddr) {
   TEST_PROGRESS("emit concurrent task end with mapped address %d\n", mappedAddr);
   Extrae_event(COMMON_EVENT_TYPE_CONCURRENT_END, mappedAddr);
}

void emit_phase_start(t_phaseID phaseID) {
   
   TEST_PROGRESS("emit start phase no: %d\n", phaseID);
   Extrae_event(COMMON_EVENT_TYPE_PHASEID, phaseID);
}


void emit_css_barrier(void) {
   
   TEST_PROGRESS("emit css barrier\n");
   Extrae_event(COMMON_EVENT_TYPE_CSS_BARRIER, 0);
}


void emit_dependency(t_taskId depending_task) {
   
   TEST_PROGRESS("emit dependency from the previous task no: %d\n", depending_task);
   Extrae_event(COMMON_EVENT_TYPE_DEPENDENCY, depending_task);
}
   
void emit_n_dependencies(t_taskId* depending_task_ptr, t_taskId n_depending_tasks) {
   int i;
   for (i=0; i<n_depending_tasks; ++i) TEST_PROGRESS("emit dependency from the previous task no: %d\n", *(depending_task_ptr+i));
   for (i=0; i<n_depending_tasks; ++i) Extrae_event(COMMON_EVENT_TYPE_DEPENDENCY, *(depending_task_ptr+i));
}