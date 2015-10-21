
#include "global.h"

typedef struct NodeInfo {
   t_taskId  last_write;
   int       array_size;
   int       spaces_occupied;
   t_taskId* array_ptr;
} NodeInfo;

/* initialize dependencies collection */
void init_dependencies_collection(void);

/* destroy dependencies collection */
void dest_dependencies_collection(void);

/* mark input access */
t_taskId mark_input(t_taskId actual_task, t_Addr addr);

/* mark output access */
t_taskId mark_output (t_taskId actual_task, t_Addr addr);

/* mark inout access */
t_taskId mark_inout (t_taskId actual_task, t_Addr addr);

void printout_all_dependencies_collection (void);
   
extern const t_taskId no_dependency_task;
