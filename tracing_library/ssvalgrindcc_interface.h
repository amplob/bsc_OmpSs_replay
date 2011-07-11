

#ifndef SS_VALGRIND_H
#define SS_VALGRIND_H

#include "global.h"


void start_task_valgrind(void *sp, char *type);
void end_task_valgrind(void);
void start_css_valgrind (void);
void end_css_valgrind (void);
void barrier_css_valgrind (void);
void wait_on_valgrind(void *ptr);

void task_input_value_valgrind(const char *parName, int value, unsigned long element_size, unsigned long elements);

void task_input_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);
void task_output_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);
void task_inout_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);

void task_unspecified_dir_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);


#endif // SS_VALGRIND_H
