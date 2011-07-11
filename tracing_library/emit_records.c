#include "emit_records.h"










/**************************************************************************
 *    FUNCTIONS THAT ARE INTERFACE TO tracing_main module
 **************************************************************************/

void emit_css_start(void) {
   TEST_PROGRESS("emit css start \n");
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_CSS_START, 1);
   MPItrace_event(COMMON_EVENT_TYPE_PHASEID, 0);   
#endif
}

void emit_css_finish(void) {
   TEST_PROGRESS("emit css finish \n");
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_CSS_START, 0);
#endif
}


void emit_task_start(t_taskcode task_code, t_taskId task_number) {
   
   TEST_PROGRESS("emit task start code %d, number %d\n",
                  task_code, task_number);
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_TASKNAME_CODED, task_code);
   MPItrace_event(COMMON_EVENT_TYPE_TASKID, task_number);
#endif
}

void emit_task_end(void) {
   
   TEST_PROGRESS("emit task end\n");
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_TASKNAME_CODED, 0);
   MPItrace_event(COMMON_EVENT_TYPE_TASKID, 0);
#endif
}


void emit_phase_start(t_phaseID phaseID) {
   
   TEST_PROGRESS("emit start phase no: %d\n", phaseID);
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_PHASEID, phaseID);
#endif
}


void emit_css_barrier(void) {
   
   TEST_PROGRESS("emit css barrier\n");
#if (!library_TESTING)
   MPItrace_event(COMMON_EVENT_TYPE_CSS_BARRIER, 0);
#endif
}