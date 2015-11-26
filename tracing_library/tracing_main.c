
#include "tracing_main.h"
#include "taskname_coder.h"
#include "dep_calculator.h"
#include "emit_records.h"


/* The global structure of this file is Execution_state f_execution_state
 * it contains the state in which is the potential smpss execution
 * it should be accessed through access functions
 */

typedef enum e_SMPSsStatus { not_started, inMainTask, inWorkingTask, finished } SMPSsStatus;

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

/* tree to store mapped addresses into ints */
static rb_red_blk_tree* all_mapped_addresses;
static int addr_count = 0;
static ActualTaskType actualTaskType= NoTask;
static int lastMappedAddr;
       

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
   
   all_mapped_addresses = RBTreeCreate ( "Address", "MappedId",
                                 addrComp, addrDest, taskIdDest, addrPrint, taskIdPrint,
                                 taskID_Get_Info, taskID_Set_Info);   
   
   /* if there is a specified file with tasknames, READ IT and put into the collection */
   tasknames_filename = getenv ("TASKNAMES");
   if (tasknames_filename == NULL) {
      printf(  "Warning: not specified file with names of the tasks: use export TASKNAMES=<path>\n"
               "The names of the tasks may be corrupted!!!!\n");
   } else {
      import_tasknames_from_file(tasknames_filename);
   }  
   
   /* emit trace record for start of the trace */
   emit_css_start();
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
   
   /* emit trace record for start of the trace */
   emit_css_finish();   
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
   emit_task_start(task_code, task_number);
}

void event_end_task (void) {
   t_taskcode code;
   
   /* leaving working task and entering main task */   
   assert (get_actual_smpss_status() == inWorkingTask);
   set_actual_smpss_status(inMainTask);
   
   /* count of executed tasks remains the same */
   /* emit actualTaskType event end */
   if(actualTaskType != NoTask) {
      if (actualTaskType == Commutative) emit_commutative_end(lastMappedAddr);
      else if (actualTaskType == Concurrent) emit_concurrent_end(lastMappedAddr);
      actualTaskType= NoTask;
   }
   
   /* code is switched to the code of the main Task */   
   TEST_PROGRESS("ending task number %d\n", 
                 get_actual_task_number());   
   code = get_code_of_main_task();
   set_actual_task_code(code);   
   
   /* emit event for ending task and starting main task again */
   emit_task_end();   
}


void event_new_phase(void) {
   t_phaseID phaseID;
   phaseID = get_actual_phaseID();
   TEST_PROGRESS("ending phase %d and starting phase %d\n",
                  phaseID, phaseID + 1);
   phaseID++;
   set_actual_phaseID(phaseID);
   
   /* emit event for marking start of new phase */
   emit_phase_start(phaseID);      
}

void event_barrier(void) {
   
   /* could restart the dependency graph */
   /* because with barrier all dependencies are reseted */
   dest_dependencies_collection();
   init_dependencies_collection();
   
   /* emit event for marking barrier */   
   emit_css_barrier(); 
}


void event_input_parameter(void *addr) {
   int i; 
   t_taskId actual_task;
   t_taskId* depending_task_ptr;
   int n_depending_tasks= 1;

   /* check sanity */
   assert(get_actual_smpss_status() == inWorkingTask);
   
   /* mark access   */
   actual_task = get_actual_task_number();
   depending_task_ptr = mark_input(actual_task, addr, &n_depending_tasks);
   
   if (*depending_task_ptr != no_dependency_task) {
      for (i=0; i<n_depending_tasks; ++i) {
	 TEST_PROGRESS("there is dependency tasks:  %d  ->   %d (INPUT) \n",
                     *(depending_task_ptr+i), actual_task);
      }
      /* if there is dependency - emit it to the trace */      
      emit_n_dependencies(depending_task_ptr, n_depending_tasks); 
   }
}


void event_output_parameter(void *addr) {
   t_taskId actual_task, depending_task;

   /* check sanity */
   assert(get_actual_smpss_status() == inWorkingTask);
   
   /* mark access   */
   actual_task = get_actual_task_number();
   depending_task = mark_output(actual_task, addr);
   
   /* now write-after-something dependencies */
   assert (depending_task == no_dependency_task);
}


void event_inout_parameter(void *addr) {
   int i;
   t_taskId actual_task;
   t_taskId* depending_task_ptr;
   int n_depending_tasks = 1;

   /* check sanity */
   assert(get_actual_smpss_status() == inWorkingTask);
   
   /* mark access   */
   actual_task = get_actual_task_number();
   depending_task_ptr = mark_inout(actual_task, addr, &n_depending_tasks);
   
   if (*depending_task_ptr != no_dependency_task) {
      for (i=0; i<n_depending_tasks; ++i) {
	 TEST_PROGRESS("there is dependency tasks:  %d  ->   %d (INOUT) \n",
                     *(depending_task_ptr+i), actual_task);
      }
      /* if there is dependency - emit it to the trace */   
      emit_n_dependencies(depending_task_ptr, n_depending_tasks); 
   }
}


/* add address to the collection    */
static void record_address (t_Addr addr, int* ni) {
   t_Addr *newAddr;
   int *newNodeInfo;
   
   /* make space for storing taskname and taskcode  */
   newAddr 	= (t_Addr*) malloc (sizeof(t_Addr));
   *newAddr = addr;
   newNodeInfo  = malloc(sizeof(int));
   *newNodeInfo = *ni;

   /* insert to the tree  */   
   RBTreeInsert(all_mapped_addresses, newAddr, newNodeInfo);
   //RBTreeSetInfo(all_mapped_addresses, newAddr, newNodeInfo);
}


void event_concurrent_commutative_parameter(void *addr, ActualTaskType c) {
   int i;
   t_taskId actual_task;
   t_taskId* depending_task_ptr;
   int n_depending_tasks = 1;
   rb_red_blk_node* AddrId;

   /* check sanity */
   assert(get_actual_smpss_status() == inWorkingTask);
   
   /* Record access to variable */
   AddrId = RBExactQuery (all_mapped_addresses, &addr);
   if (AddrId == NO_NODE_FOUND) {
      record_address(addr, &addr_count);
      ++addr_count;
      AddrId = RBExactQuery (all_mapped_addresses, &addr);
   }
   
   /* emit ActualTaskType event start */
   actualTaskType= c;
   lastMappedAddr= *((int*)((rb_red_blk_node*)AddrId)->info);
   if (c == Commutative) emit_commutative_start(lastMappedAddr);
   else if (c == Concurrent) emit_concurrent_start(lastMappedAddr);
   else printf("not reading ActualTaskType correctly\n");
   
   
   /* mark access */
   actual_task = get_actual_task_number();
   depending_task_ptr = mark_concurrent_commutative(actual_task, addr, &n_depending_tasks, c);
  
   if (*depending_task_ptr != no_dependency_task) {
      for (i=0; i<n_depending_tasks; ++i) {
	 TEST_PROGRESS("there is dependency tasks:  %d  ->   %d (INOUT) \n",
                     *(depending_task_ptr+i), actual_task);
      }
      /* if there is dependency - emit it to the trace */   
      emit_n_dependencies(depending_task_ptr, n_depending_tasks); 
   }
}


void event_wait_on(void *addr) {
   int i;
   t_taskId actual_task;
   t_taskId* depending_task_ptr;
   int n_depending_tasks = 1;
   /* check sanity */
   assert(get_actual_smpss_status() == inMainTask);
   
   /* mark access from main */
   /* this is FAKE, bacause mark_input will not mark any access */
   /* it will just detect dependency, so actual_task is irrelevant */
   actual_task = 0;
   depending_task_ptr = mark_input(actual_task, addr, &n_depending_tasks);
   
   if (*depending_task_ptr != no_dependency_task) {
      for (i=0; i<n_depending_tasks; ++i) {
	 TEST_PROGRESS("there is dependency tasks:  %d  ->   %d (WAIT-ON) \n",
                     *(depending_task_ptr+i), actual_task);
      }
      /* if there is dependency - emit it to the trace */      
      emit_n_dependencies(depending_task_ptr, n_depending_tasks); 
   }   
}
