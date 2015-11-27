
#include "global.h"

typedef struct NodeInfo {
   t_taskId        last_write;
   int             array_size;
   int             spaces_occupied;
   ActualTaskType  type;
   t_taskId*       array_ptr;
} NodeInfo;

extern t_taskId* array_aux;

/* initialize dependencies collection */
void init_dependencies_collection(void);

/* destroy dependencies collection */
void dest_dependencies_collection(void);

/* mark input access */
t_taskId* mark_input (t_taskId actual_task, t_Addr addr, int* n_depending_tasks);

/* mark output access */
t_taskId mark_output (t_taskId actual_task, t_Addr addr);

/* mark inout access */
t_taskId* mark_inout (t_taskId actual_task, t_Addr addr, int* n_depending_tasks);

/* mark commutative access */
t_taskId* mark_commutative (t_taskId actual_task, t_Addr addr, int* n_depending_tasks);

/* mark concurrent and commutative access */
t_taskId* mark_concurrent_commutative (t_taskId actual_task, t_Addr addr, int* n_depending_tasks, ActualTaskType c);

void printout_all_dependencies_collection (void);
   
extern const t_taskId no_dependency_task;

/* unused but necessary functions for tree creations outside dep_caculator */
int addrComp(const void* a,const void* b);
void addrPrint(const void* a);
void taskIdPrint(void* a);
void addrDest(void* a);
void taskIdDest(void *a);
void* taskID_Get_Info (void* record);
void taskID_Set_Info (void* record, void* new_info);

