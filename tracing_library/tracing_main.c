
#include "tracing_main.h"


/* The global structure of this file is Execution_state f_execution_state
 * it contains the state in which is the potential smpss execution
 * it should be accessed through access functions
 */

typedef enum e_SMPSsStatus { not_started, inMainTask, inWorkingTask, finished } SMPSsStatus;

typedef struct execution_state {
   SMPSsStatus smpss_status;
   t_taskId    taskCode;
   t_taskNo    tasksExecutedSoFar;
} Execution_state;

/* STATE OF THE LIBRARY */
static Execution_state f_execution_state =
                  {  .smpss_status = not_started,
                     .taskCode = 0,
                     .tasksExecutedSoFar   = 0,
                  };
                  
/* access functions for f_execution_state  */                  
static inline SMPSsStatus get_actual_smpss_status (void) {
   return f_execution_state.smpss_status;
}

static inline void set_actual_smpss_status (SMPSsStatus status) {
   f_execution_state.smpss_status = status;
}

static inline t_taskId get_actual_task_code (void) {
   return f_execution_state.taskCode;
}

static inline void set_actual_task_code (t_taskId task_code) {
   f_execution_state.taskCode = task_code;
}

static inline t_taskNo get_actual_task_number (void) {
   return f_execution_state.tasksExecutedSoFar;
}

static inline void set_actual_task_number (t_taskNo task_number) {
   f_execution_state.tasksExecutedSoFar = task_number;
}
   
   



void event_start_css(void) {
   /* change the state of the main structure */
   assert (get_actual_smpss_status() == not_started);
   set_actual_smpss_status(inMainTask);
   assert (get_actual_task_code() == 0);  /* because it is in main task for sure */
   assert (get_actual_task_number() == 0);
}


void event_end_css(void) {
   /* change the state of the main structure */
   assert (get_actual_smpss_status() == inMainTask);
   set_actual_smpss_status(finished);
   if (get_actual_task_number() == 0)
      printf ("WARNING: ending css section, total number of tasks in this rank is 0\n");
   else 
      printf ("total number of tasks in this rank is %d\n",
                  get_actual_task_number());
}
                  

void event_start_task (const char * taskname) {
   
   
}

void event_end_task (void) {
   
   
}


