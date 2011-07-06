#include "taskname_coder.h"


#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>

/* track if the library is correctly initialized and destructed */
static LibraryStatus libraryStatus = uninitialized;

/* track if there were unspecified tasknames */
static bool exist_nonspecified_tasknames;        

/* incrementaly assignment of the taskname codes */
/* to be reseted when the taskname_coding_library gets initialized */
static t_taskcode actual_code;

/* trees to store tasknames and their codes */
static rb_red_blk_tree* all_tasknames;
static rb_red_blk_tree* nonspecified_tasknames;


/* -------------------------------------------
 * functions to initialize the trees
 * ------------------------------------------- */

int tasknameComp(const void* a,const void* b) {
   int res = strncmp(a, b, MAX_TASKNAME_LEN);
   if (res < 0)
      return -1;
   else if (res == 0)
      return 0;
   else return 1;
}

void tasknamePrint(const void* a) {
  printf("%s", (char *)a);
}

void taskcodePrint(void* a) {
  printf("%d", *(t_taskcode*)a);
}

void tasknameDest(void* a) {
  free((int*)a);
}

void taskcodeDest(void *a){
  free((t_taskcode*)a);
}


/* -------------------------------------------
 * internal logic of the library
 * ------------------------------------------- */

inline static t_taskcode get_new_taskname_code(void) {
   actual_code++;
   return actual_code;
}

/* allocate new memory and copy the content there */
inline static t_taskcode* make_persistant_taskcode (t_taskcode code){
   t_taskcode *newCode;
   newCode = (t_taskcode*) malloc(sizeof(t_taskcode));
   *newCode = code;   
   return newCode;
}

/* allocate new memory and copy the content there */
inline static char* make_persistant_taskname(char *task_name) {
   char *newName;
   newName=(char*) malloc(sizeof(char) * MAX_TASKNAME_LEN);
   strncpy(newName, task_name, MAX_TASKNAME_LEN);
   return newName;
}

/* add taskname [task_name, task_code] to tree  
 * check that there is no repetition of task names
 */
static void add_taskname_to_tree(char* task_name, t_taskcode task_code, rb_red_blk_tree* tree) {
   char *newName;
   t_taskcode *newCode;
   
#ifdef TESTING   
   /* make sure there is NOT already present taskname  */
   rb_red_blk_node *newNode;
   newNode = RBExactQuery( tree, task_name);
   if (newNode != NULL) 
      PANIC("duplication of tasknames, repeated taskname: new->%s and old->%s\n",
            task_name,  (char *)(newNode->key));
#endif   
   
   /* make space for storing taskname and taskcode  */
   newName  = make_persistant_taskname(task_name);
   newCode  = make_persistant_taskcode(task_code);   
     
   /* insert to the tree  */   
   RBTreeInsert(tree, newName, newCode);   
   
}



/*-------------------------------------
       INTERFACE FUNCTIONS
 -------------------------------------- */

/* initialize tasknames collection */
void init_tasknames_collection(void) {
   assert(libraryStatus == uninitialized);
   actual_code = 0;
   exist_nonspecified_tasknames = False;
   libraryStatus = initialized;
   all_tasknames = RBTreeCreate ( "taskname", "taskname_code",
                                 tasknameComp, tasknameDest, taskcodeDest, tasknamePrint, taskcodePrint,
                                NULL, NULL);   
   nonspecified_tasknames = RBTreeCreate ( "taskname", "taskname_code",
                                 tasknameComp, tasknameDest, taskcodeDest, tasknamePrint, taskcodePrint,
                                NULL, NULL);   
}

/* destroy tasknames collection */
void dest_tasknames_collection(void) {
   assert(libraryStatus == initialized);
   RBTreeDestroy(all_tasknames);
   RBTreeDestroy(nonspecified_tasknames);     
   libraryStatus = uninitialized;
}

/* insert new taskname
 * and assigning to it a new code  */
void add_taskname(char* task_name) {
   t_taskcode task_code;
   assert(libraryStatus == initialized);      
   task_code = get_new_taskname_code();
   add_taskname_to_tree(task_name, task_code, all_tasknames);
}

/* code the taskname */
t_taskcode find_taskcode(char *task_name) {
   rb_red_blk_node *newNode;
   assert(libraryStatus == initialized);      
   
   if ( ( newNode = RBExactQuery( all_tasknames, task_name) ) ) {
      /* if already present in the collection */
      return *(t_taskcode*) (newNode->info);
   } else {
      /* still not present in the collection */
      
      /* add it to the collection of tasknames */      
      t_taskcode task_code;
      task_code = get_new_taskname_code();
      add_taskname_to_tree(task_name, task_code, all_tasknames);

      /* add it to the collection of nonspecified tasknames */      
      exist_nonspecified_tasknames = True;
      add_taskname_to_tree(task_name, task_code, nonspecified_tasknames);
      
      /* now it is already present in the tree */      
      newNode = RBExactQuery( all_tasknames, task_name);
      assert(newNode);
      return *(t_taskcode*) (newNode->info);
   }
}

/* print out all tasknames */
void printout_all_tasknames(void) {
   assert(libraryStatus == initialized);   
   printf("collection of tasknames and the corresponding codes is \n");
   RBTreePrint(all_tasknames);
   printf("\n");   
}

/* print out all NONSPECIFIED tasknames */
void printout_nonspecified_tasknames(void) {
   assert(libraryStatus == initialized);   
   if (exist_nonspecified_tasknames) {
      printf("\nWARNING !!!! \n\n");
      printf("collection of NONSPECIFIED tasknames and the corresponding codes is \n");   
      RBTreePrint(nonspecified_tasknames);
      printf("\n");      
   }
}





