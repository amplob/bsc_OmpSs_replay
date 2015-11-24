#include "global.h"
#include "common-global.h"


void emit_css_start(void);
void emit_css_finish(void);

void emit_task_start(t_taskcode task_code, t_taskId task_number);
void emit_task_end(void);

void emit_commutative_start(int addr);
void emit_commutative_end(int addr);

void emit_phase_start(t_phaseID phaseID);

void emit_css_barrier(void);

void emit_dependency(t_taskId depending_task);
void emit_n_dependencies(t_taskId* depending_task_ptr, t_taskId n_depending_tasks);
