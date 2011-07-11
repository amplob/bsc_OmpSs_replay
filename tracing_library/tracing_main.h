
#include "global.h"

void event_start_task (const char * taskname);
void event_end_task (void);

void event_start_css(void);
void event_end_css(void);

void event_new_phase(void);

void event_barrier(void);

void event_input_parameter(void *ptr);
void event_output_parameter(void *ptr);
void event_inout_parameter(void *ptr);