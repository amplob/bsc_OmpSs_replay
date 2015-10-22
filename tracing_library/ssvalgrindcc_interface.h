

#ifndef SS_VALGRIND_H
#define SS_VALGRIND_H

#ifdef __cplusplus
extern "C"{
#endif

void replay_start_task(void);
void replay_end_task(void);

void wait_fake(void *ptr);

void start_css_valgrind (void);
void end_css_valgrind (void);
void barrier_css_valgrind (void);
void wait_on_valgrind(void *ptr);


//void replay_input_task_value(const char *parName, int value, unsigned long element_size, unsigned long elements);

void replay_input_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);

void replay_output_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);
void replay_inout_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);
void replay_commutative_task(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);

void task_unspecified_dir_valgrind(const char *parName, void *ptr, unsigned long element_size, unsigned long elements);


void start_new_phase_valgrind(void);

#ifdef __cplusplus
}
#endif

#endif // SS_VALGRIND_H
