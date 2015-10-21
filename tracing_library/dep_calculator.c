#include "dep_calculator.h"


#include<stdio.h>
#include<ctype.h>
#include<string.h>


/* track if the library is correctly initialized and destructed */
static LibraryStatus libraryStatus = uninitialized;

/* trees to store tasknames and their codes */
static rb_red_blk_tree* all_dependent_memory_references;

const t_taskId no_dependency_task = -1;

/* -------------------------------------------
 * functions to initialize the trees
 * ------------------------------------------- */

int addrComp(const void* a,const void* b) {
   if (*(t_Addr *)a < *(t_Addr *)b)
      return -1;
   else if (*(t_Addr *)a > *(t_Addr *)b)
      return 1;
   else return 0;
}

void addrPrint(const void* a) {
  printf("%p", *(t_Addr *)a);
}

void taskIdPrint(void* a) {
  printf("%d", *(t_taskId *)a);
}

void addrDest(void* a) {
  free((t_Addr*)a);
}

void taskIdDest(void *a){
  free((t_taskId*)a);
}

void* taskID_Get_Info (void* record) {
   // now dealing with struct
   return  ((rb_red_blk_node*)record)->info; 
}

void taskID_Set_Info (void* record, void* new_taskId) {
   // now dealing with struct
   t_taskId *old_taskId;
   old_taskId = ((rb_red_blk_node*)record)->info;
   *old_taskId =  *((t_taskId*) new_taskId); 
}

/* -------------------------------------------
 * internal logic of the library
 * ------------------------------------------- */

/* allocate new memory and copy the content there */
static t_Addr* make_persistant_addr (t_Addr addr){
   t_Addr *newAddr;
   newAddr = (t_Addr*) malloc (sizeof(t_Addr));
   *newAddr = addr;
   return newAddr;
}


/* allocate new memory and copy the content there */
static t_taskId* make_persistant_taskId (t_taskId taskId) {
   t_taskId *new_taskId;
   new_taskId = (t_taskId*) malloc (sizeof(t_taskId));
   *new_taskId = taskId;
   return new_taskId;
}


/* find the node in the collection that records the last write
 * inside this address                                            */
static rb_red_blk_node* find_previous_write_record(t_Addr addr) {
   return RBExactQuery (all_dependent_memory_references, &addr);
}


/* add write record [addr, taskId] to the collection    */
static void record_write (t_Addr addr, t_taskId taskId) {
   t_Addr *newAddr;
   t_taskId *newTaskId;
   
#if library_TESTING
   /* make sure there is NOT already present taskname  */
   rb_red_blk_node *newNode;
   newNode = RBExactQuery( all_dependent_memory_references, &addr);
   if (newNode != NULL) 
      PANIC("duplication of addresses in the dependency tree, repeated Address: new->%p and old->%p\n",
            addr,  *(t_Addr *)(newNode->key));
#endif   
   
   /* make space for storing taskname and taskcode  */
   newAddr  = make_persistant_addr(addr);
   newTaskId  = make_persistant_taskId(taskId);   
     
   /* insert to the tree  */   
   RBTreeInsert(all_dependent_memory_references,
                newAddr, newTaskId);   
}



/*-------------------------------------
       INTERFACE FUNCTIONS
 -------------------------------------- */

/* initialize dependencies collection */
void init_dependencies_collection(void) {
   assert(libraryStatus == uninitialized);
   libraryStatus = initialized;
   
   all_dependent_memory_references = RBTreeCreate ( "Address", "lastWrite taskId",
                                 addrComp, addrDest, taskIdDest, addrPrint, taskIdPrint,
                                 taskID_Get_Info, taskID_Set_Info);   
}


/* destroy dependencies collection */
void dest_dependencies_collection(void) {
   assert(libraryStatus == initialized);
   RBTreeDestroy(all_dependent_memory_references);
   libraryStatus = uninitialized;
}


/* mark input access */
t_taskId mark_input(t_taskId actual_task, t_Addr addr) {
   rb_red_blk_node *previous_write_taskID_node;
   /* assume there will be no dependency */
   t_taskId caught_dependency = no_dependency_task;
   assert(libraryStatus == initialized);     
   
   /* find if there is dependency from some previous task */
   previous_write_taskID_node = find_previous_write_record (addr);
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      caught_dependency =  * ((t_taskId*) RBTreeGetInfo(all_dependent_memory_references, 
                                                        previous_write_taskID_node));    
   } else {
      /* just know that nothing is found, will return no_dependency_task */
   }
   
   /* return the caught dependency*/
   return caught_dependency;   
}


/* mark output access */
t_taskId mark_output (t_taskId actual_task, t_Addr addr) {
   rb_red_blk_node *previous_write_taskID_node;
   assert(libraryStatus == initialized);     
   
   /* update the last task that has writen to this memory reference */
   previous_write_taskID_node = find_previous_write_record (addr);
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      RBTreeSetInfo(all_dependent_memory_references,
                    previous_write_taskID_node,
                    &actual_task);
   } else {
      record_write (addr, actual_task);
   }
   
   /* return no dependency, because of buffer renaming */
   return no_dependency_task;
}


// struct {
//    taskID last_write;
//    int    array_size;
//    int    spaces_occupied;
//    *taskID array_ptr;
// }

/* mark inout access */
t_taskId mark_inout (t_taskId actual_task, t_Addr addr) {
   rb_red_blk_node *previous_write_taskID_node;
   /* assume there will be no dependency */
   t_taskId caught_dependency = no_dependency_task;
   assert(libraryStatus == initialized);        
   
   /* find if there is dependency from some previous task */
   previous_write_taskID_node = find_previous_write_record (addr);
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      /* record the dependency and update the record in the collection */
      // caught dependencies: 1task_ID + array of taskIDs (size)
      caught_dependency =  * ((t_taskId*) RBTreeGetInfo(all_dependent_memory_references, 
                                                        previous_write_taskID_node));    
      RBTreeSetInfo(all_dependent_memory_references,
                    previous_write_taskID_node,
                    &actual_task);
   } else {
      /* make new record in the collection */
      /* and there is no dependency to report */      
      record_write (addr, actual_task);
   }   
      
   /* return the caught dependency*/
   return caught_dependency;
}

void printout_all_dependencies_collection (void) {
   assert(libraryStatus == initialized);   
   printf("collection of dependencies \n");
   RBTreePrint(all_dependent_memory_references);
   printf("\n");
}





