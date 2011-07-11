
#include "tracing_main.h"
#include "taskname_coder.h"
#include "dep_calculator.h"


/* The global structure of this file is Execution_state f_execution_state
 * it contains the state in which is the potential smpss execution
 * it should be accessed through access functions
 */

typedef enum e_SMPSsStatus { not_started, inMainTask, inWorkingTask, finished } SMPSsStatus;

typedef int t_phaseID;                  

typedef struct execution_state {
   SMPSsStatus smpss_status;
   t_taskcode  taskCode;
   t_taskId    tasksExecutedSoFar;
   t_phaseID   phaseID;
} Execution_state;

/* STATE OF THE LIBRARY */
static Execution_state f_execution_state =
                  {  .smpss_status = not_started,
                     .taskCode = 0,
                     .tasksExecutedSoFar   = 0,
                     .phaseID = 0,
                  };
       

/* access functions for f_execution_state  */                  
static inline SMPSsStatus get_actual_smpss_status (void) {
   return f_execution_state.smpss_status;
}

static inline void set_actual_smpss_status (SMPSsStatus status) {
   f_execution_state.smpss_status = status;
}

static inline t_taskcode get_actual_task_code (void) {
   return f_execution_state.taskCode;
}

static inline void set_actual_task_code (t_taskcode task_code) {
   f_execution_state.taskCode = task_code;
}

static inline t_taskId get_actual_task_number (void) {
   return f_execution_state.tasksExecutedSoFar;
}

static inline void set_actual_task_number (t_taskId task_number) {
   f_execution_state.tasksExecutedSoFar = task_number;
}
   
static inline void set_actual_phaseID (t_phaseID phaseID) {
   f_execution_state.phaseID = phaseID;
}

static inline t_phaseID get_actual_phaseID (void) {
   return f_execution_state.phaseID;
}


/**************************************************************************
 *    FUNCTIONS THAT ARE INTERFACE TO ssvalgrindcc_interface module
 **************************************************************************/
void event_start_css(void) {
   char *tasknames_filename;
   
   /* change the state of the main structure */
   assert (get_actual_smpss_status() == not_started);
   set_actual_smpss_status(inMainTask);
   assert (get_actual_task_code() == 0);      /* because it is in main task for sure */
   assert (get_actual_task_number() == 0);
   
   /* initialize collections */
   init_tasknames_collection();
   init_dependencies_collection();
   
   /* if there is a specified file with tasknames, READ IT and put into the collection */
   tasknames_filename = getenv ("TASKNAMES");
   TEST_PROGRESS("to read task names from the file    %s\n", tasknames_filename);
   if (tasknames_filename == NULL) {
      printf(  "Warning: not specified file with names of the tasks: use export TASKNAMES=<path>\n"
               "The names of the tasks may be corrupted!!!!\n");
   } else {
      import_tasknames_from_file(tasknames_filename);
   }
   
}


void event_end_css(void) {
   /* change the state of the main structure */
   assert (get_actual_smpss_status() == inMainTask);
   set_actual_smpss_status(finished);
   
   /* some statistics of the execution */
   printf("\n");
   printf("-----------------------------------------------------------------------------------\n");
   printf("----------------------MPISs tracing completed--------------------------------------\n");
   printf("-----------------------------------------------------------------------------------\n");
   if (get_actual_task_number() == 0)
      printf ("WARNING: ending css section, total number of tasks in this rank is 0\n");
   else 
      printf ("total number of tasks in this rank is %d\n",
                  get_actual_task_number());
      
   if (get_actual_phaseID() == 0) {
      printf("WARNING: you marked no phases?!\n");
   } else {
      printf("availible phases [0 - %d]\n", get_actual_phaseID());
   }
   /* inform if there were some task names that are not specified */
   printout_nonspecified_tasknames();   
   printf("-----------------------------------------------------------------------------------\n");      
   printf("\n");
   
   /* make pcf file with tasknames*/
   flush_tasknames_pcf("tasknames.pcf");
   
   /* destroy collections */
   dest_dependencies_collection();   
   dest_tasknames_collection(); 
}
                  

void event_start_task (const char * taskname) {
   t_taskcode task_code;
   t_taskId task_number;
   
   /* entering working task */   
   assert (get_actual_smpss_status() == inMainTask);
   set_actual_smpss_status(inWorkingTask);
   
   /* count number of executed tasks */
   task_number = get_actual_task_number() + 1;
   set_actual_task_number(task_number);
   
   /* change the code of the actual task from 1(main) to something (working task) */
   task_code = find_taskcode(taskname);
   set_actual_task_code(task_code);
   TEST_PROGRESS("starting task number %d  name is %s which is coded into %d \n", 
                 get_actual_task_number(),
                 taskname,
                 task_code);
   
   /* emit event for starting task */
}

void event_end_task (void) {
   t_taskcode code;
   
   /* leaving working task and entering main task */   
   assert (get_actual_smpss_status() == inWorkingTask);
   set_actual_smpss_status(inMainTask);
   
   /* count of executed tasks remains the same */
   
   /* code is switched to the code of the main Task */   
   TEST_PROGRESS("ending task number %d\n", 
                 get_actual_task_number());   
   code = get_code_of_main_task();
   set_actual_task_code(code);   
   
   /* emit event for ending task and starting main task again */
   
}


void event_new_phase(void) {
   t_phaseID phaseID;
   phaseID = get_actual_phaseID();
   TEST_PROGRESS("ending phase %d and starting phase %d\n",
                  phaseID, phaseID + 1);
   phaseID++;
   set_actual_phaseID(phaseID);
   
   /* emit event for marking start of new phase */
   
}

void event_barrier(void) {
   
   /* could empty the dependency graph */   
   dest_dependencies_collection();
   init_dependencies_collection();
   
   /* emit event for marking barrier */   
}


