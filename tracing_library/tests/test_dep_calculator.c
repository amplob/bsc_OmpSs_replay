#include "dep_calculator.h"
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

void test_interactive(void) {
  int option=0;   
   
  while(option!=6) {
    printf("choose one of the following:\n");
    printf("(1) initialize \n(2) mark_input \n(3) mark_output\n");
    printf("(4) mark_inout \n(5) printout collection \n(6) destroy and leave \n");
    
    do option=fgetc(stdin); while(-1 != option && isspace(option));
    option-='0';
    switch(option)
      {
      case 1:
   {
      init_dependencies_collection();
   }
   break;
      case 2:
   {
     t_taskId taskId;
     t_Addr addr;
     printf("type taskid and address\n");
     scanf("%d %p", &taskId, &addr);
     mark_input(taskId, addr);
   }
   break;   
      case 3:
   {
     t_taskId taskId;
     t_Addr addr;
     printf("type taskid and address\n");
     scanf("%d %p", &taskId, &addr);
     mark_output(taskId, addr);
   }
   break;
      case 4:
   {
     t_taskId taskId;
     t_Addr addr;
     printf("type taskid and address\n");
     scanf("%d %p", &taskId, &addr);
     mark_inout(taskId, addr);
   }
   break;   
      case 5:
   {
     printf("the whole collection of memory references is \n");
     printout_all_dependencies_collection();
   }
   break;    
      case 6:
   {
      dest_dependencies_collection();
   }
   break;   
   break;
      default:
   printf("Invalid input; Please try again.\n");
      }
  }  
}

static void simple_raw_dependency () {
   t_taskId old_taskId;
   init_dependencies_collection();
   
   printf("start simple_raw_dependency  TEST \n");
   
   old_taskId = mark_input (1, (void *) 100);
   assert (old_taskId == -1);
   
   old_taskId = mark_output(1, (void *) 100);
   assert (old_taskId == -1);
   old_taskId = mark_input (2, (void *) 100);
   assert (old_taskId == 1);
   
   old_taskId = mark_output (2, (void *) 101);
   assert (old_taskId == -1);
   old_taskId = mark_input (3, (void *) 101);
   assert (old_taskId == 2);
   
   old_taskId = mark_inout (3, (void*) 101);
   assert (old_taskId == 2);
   old_taskId = mark_input (4, (void *) 101);
   assert (old_taskId == 3);
   
   printout_all_dependencies_collection();

   printf("end simple_raw_dependency  TEST \n\n\n");   
   dest_dependencies_collection();
}

static void bigger_dep_collection () {

   t_taskId old_taskId;
   init_dependencies_collection();
   
   printf("start bigger_dep_collection  TEST \n");   
   
   old_taskId = mark_input (1, (void *) 100);
   assert (old_taskId == -1);
   old_taskId = mark_input (1, (void *) 200);
   assert (old_taskId == -1);   
   old_taskId = mark_input (1, (void *) 300);
   assert (old_taskId == -1);   
   
   
   old_taskId = mark_output (0, (void *) 100);
   assert (old_taskId == -1);
   old_taskId = mark_output (1, (void *) 101);
   assert (old_taskId == -1);
   old_taskId = mark_output (2, (void *) 102);
   assert (old_taskId == -1);    
   old_taskId = mark_output (3, (void *) 103);
   assert (old_taskId == -1);     
   old_taskId = mark_output (4, (void *) 104);
   assert (old_taskId == -1);      
   old_taskId = mark_output (5, (void *) 105);
   assert (old_taskId == -1);      
   old_taskId = mark_output (6, (void *) 106);
   assert (old_taskId == -1);     
   old_taskId = mark_output (7, (void *) 107);
   assert (old_taskId == -1);      
   old_taskId = mark_output (8, (void *) 108);
   assert (old_taskId == -1);      
   old_taskId = mark_output (9, (void *) 109);
   assert (old_taskId == -1);      

   old_taskId = mark_input (10, (void *) 100);
   assert (old_taskId == 0);
   old_taskId = mark_input (11, (void *) 101);
   assert (old_taskId == 1);         
   old_taskId = mark_input (12, (void *) 102);
   assert (old_taskId == 2);         
   old_taskId = mark_input (13, (void *) 103);
   assert (old_taskId == 3);         
   old_taskId = mark_input (14, (void *) 104);
   assert (old_taskId == 4);            
   
   old_taskId = mark_inout (25, (void *) 105);
   assert (old_taskId == 5);               
   old_taskId = mark_inout (26, (void *) 106);
   assert (old_taskId == 6);               
   old_taskId = mark_inout (27, (void *) 107);
   assert (old_taskId == 7);               
   old_taskId = mark_inout (28, (void *) 108);
   assert (old_taskId == 8);               
   old_taskId = mark_inout (29, (void *) 109);
   assert (old_taskId == 9);               
   
   old_taskId = mark_input (35, (void *) 105);
   assert (old_taskId == 25);                  
   old_taskId = mark_input (36, (void *) 106);
   assert (old_taskId == 26);                  
   old_taskId = mark_input (37, (void *) 107);
   assert (old_taskId == 27);                  
   old_taskId = mark_input (38, (void *) 108);
   assert (old_taskId == 28);                  
   old_taskId = mark_input (39, (void *) 109);
   assert (old_taskId == 29);                  
   
   printout_all_dependencies_collection();
   
   printf("end bigger_dep_collection  TEST \n\n\n");      
   dest_dependencies_collection();
}

static void test_automatic() {
   
   simple_raw_dependency ();
   
   bigger_dep_collection();
   
   
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
