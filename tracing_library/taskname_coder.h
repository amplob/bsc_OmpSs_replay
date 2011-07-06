
#include "global.h"

void init_tasknames_collection(void);
void dest_tasknames_collection(void);

void add_taskname(char* task_name);
t_taskcode find_taskcode(char *task_name);

void printout_all_tasknames(void);
void printout_nonspecified_tasknames(void);