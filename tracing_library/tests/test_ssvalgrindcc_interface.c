

#include "dep_calculator.h"
#include "ssvalgrindcc_interface.h"
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>

#include <errno.h>

#include <execinfo.h>
#include <signal.h>

void print_backtrace (int a) {

  /* prevent infinite recursion if print_backtrace() causes another segfault   */
  void *array[100];
  int size;
  char **strings;
  int i;

  signal(SIGSEGV, SIG_DFL);
  signal(SIGABRT, SIG_DFL);
  
  size = backtrace (array, 100);
  strings = backtrace_symbols (array, size);

  printf ("%d\n", a);
  printf("Stack frames (%d):\n", size);

  for (i = 0; i < size; i++)
    printf( "%s\n", strings[i]);

  free (strings);
}

/* Segmentation fault signal handler.  */
void
segfaultHandler(int sigtype)
{
    printf("Segmentation fault \n");
    print_backtrace(sigtype);
    exit(-1);
}

/* THIS IS TO BE PUT IN THE MAIN FUNCTION AT THE START */
/*  signal(SIGSEGV, segfaultHandler);  */


static void test_interactive() {
   
}

#pragma css task input(A)
static void input_compute(float A[200]) {
   A[1] = 0;
}

#pragma css task input(A)
static void compute_input(float A[200]) {
   A[1] = 0;
}

#pragma css task output(A)
static void output_compute(float A[200]) {
   A[1] = 0;
}

#pragma css task output(A)
static void compute_output(float A[200]) {
   A[1] = 0;
}

#pragma css task inout(A)
static void inout_compute(float A[200]) {
   A[1] = 0;
}

static void test_automatic() {
   
#pragma css start
   float buff[1000];
   compute_input(buff);
   
   start_new_phase_valgrind();
   
   compute_output(buff);
   input_compute(buff);
   
   start_new_phase_valgrind();   
   
   output_compute(buff);
   inout_compute(buff);
   
   
#pragma css wait on (buff)   

#pragma css barrier

#pragma css finish
}


int main(int argc, char *argv[]) {

  signal(SIGSEGV, segfaultHandler);     
   
  /* read options in the while loop
   */
  if (argc == 1) {
     test_automatic();
  } else if  (argc == 2){
      if (strncmp (argv[1], "-i", MAX_TASKNAME_LEN) == 0) 
         test_interactive();
  }
  return 0;
}
