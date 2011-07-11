
#include "global.h"

void init_tasknames_collection(void);
void dest_tasknames_collection(void);

void add_taskname(char* task_name);
t_taskcode find_taskcode(char *task_name);

void import_tasknames_from_file(char *filename);
void flush_tasknames_pcf(char *filename);

void printout_all_tasknames(void);
void printout_nonspecified_tasknames(void);