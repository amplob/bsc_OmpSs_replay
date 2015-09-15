#include "taskname_coder.h"
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>

void test_interactive(void) {
  int option=0;   
   
  while(option!=8) {
    printf("choose one of the following:\n");
    printf("(1) add to tree\n(2) delete from tree\n(3) query\n");
    printf("(4) find predecessor\n(5) find sucessor\n(6) enumerate\n");
    printf("(7) print tree\n(8) quit\n");
    do option=fgetc(stdin); while(-1 != option && isspace(option));
    option-='0';
    switch(option)
      {
      case 1:
   {
     char task_name[MAX_TASKNAME_LEN];      
     printf("type key for new node\n");
     scanf("%s",task_name);
     add_taskname(task_name);
   }
   break;
   
      case 3:
   {
     char task_name[MAX_TASKNAME_LEN]; 
     t_taskcode task_code;
     printf("type key of node to query for\n");
     scanf("%s",task_name);
     task_code = find_taskcode (task_name);
     printf("found taskcode is %d\n", task_code);
   }
   break;
      case 7:
   {
     printf("all the tasknames\n");
     printout_all_tasknames();
     printf("nonspecified tasknames\n");     
     printout_nonspecified_tasknames();
   }
   break;
      case 8:
   {
     dest_tasknames_collection();
     return;
   }
   break;
      default:
   printf("Invalid input; Please try again.\n");
      }
  }  
}

static void empty_name () {
   t_taskcode code;
   printf("start empty_name TEST\n");         
   init_tasknames_collection();

   code = find_taskcode("");
   assert(code == 1);
   
   printout_all_tasknames();
   printout_nonspecified_tasknames();   
   dest_tasknames_collection();
   printf("end empty_name TEST\n\n\n");         
}

static void tens_of_transactions() {
   t_taskcode code;
   printf("start tens_of_transactions TEST\n");   
   
   init_tasknames_collection();
   
   add_taskname("task1");
   add_taskname("task2");
   add_taskname("task3");
   add_taskname("task4");
   add_taskname("task5");
   add_taskname("task6");
   add_taskname("task7");
   code = find_taskcode("task8");
   assert (code == 8);
   code = find_taskcode("task9");
   assert (code == 9);
   
   code = find_taskcode("task3");
   assert (code == 3);
   
   code = find_taskcode("task2");
   assert (code == 2);   
   
   code = find_taskcode("task1");
   assert (code == 1);
   
   printout_all_tasknames();
   printout_nonspecified_tasknames();
   dest_tasknames_collection();
   
   printf("end tens_of_transactions TEST\n\n\n");   
}

static void reading_file (const char *filename) {
   char taskname[MAX_TASKNAME_LEN];
   FILE *file;
   printf("start reading file TEST\n\n\n");               
   
   init_tasknames_collection();
   
   file = fopen (filename, "r");
   if (file == NULL) {
      PANIC("Unable to open file with taskname %s\n", filename); 
   }
   
   while (fscanf(file, "%s", taskname) != EOF) {
      add_taskname(taskname);
   }

   printout_all_tasknames();
   printout_nonspecified_tasknames();
   dest_tasknames_collection();
   printf("end reading file TEST\n\n\n");         
}

static void test_automatic() {
   
   empty_name();
   tens_of_transactions();
   reading_file("tasknames.txt");
   
   
}

int main(int argc, char *argv[]) {

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
