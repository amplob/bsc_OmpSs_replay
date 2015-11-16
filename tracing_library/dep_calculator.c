#include "dep_calculator.h"


#include<stdio.h>
#include<ctype.h>
#include<string.h>


/* track if the library is correctly initialized and destructed */
static LibraryStatus libraryStatus = uninitialized;

/* trees to store tasknames and their codes */
static rb_red_blk_tree* all_dependent_memory_references;

const t_taskId no_dependency_task = -1;

extern t_taskId* array_aux = NULL;


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
  free((NodeInfo*)a);
}

void* taskID_Get_Info (void* record) {
   return  ((rb_red_blk_node*)record)->info; 
}

void taskID_Set_Info (void* record, void* new_info) {
   // now dealing with struct
   NodeInfo *old_info;
   old_info = (NodeInfo*)((rb_red_blk_node*)record)->info;
   old_info->last_write = ((NodeInfo*)new_info)->last_write;
   old_info->spaces_occupied = ((NodeInfo*)new_info)->spaces_occupied;
   old_info->array_size = ((NodeInfo*)new_info)->array_size;
   //(old_info->array_ptr) = ((NodeInfo*)new_info)->array_ptr;
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
static NodeInfo* make_persistant_taskId (NodeInfo* ni) {
   int i;
   NodeInfo *new_ni;
   new_ni = (NodeInfo*) malloc (sizeof(NodeInfo));
   new_ni->last_write = ni->last_write;
   new_ni->array_size = ni->array_size;
   new_ni->spaces_occupied = ni->spaces_occupied;
   new_ni->array_ptr = ni->array_ptr; //if ni->array_ptr is NULL then new_ni->array_ptr is NULL
   if (ni->array_ptr) {
      new_ni->array_ptr = malloc (ni->array_size * sizeof(t_taskId));
      for (i=0; i<ni->spaces_occupied; ++i) *(new_ni->array_ptr + i) = *(ni->array_ptr + i);
   }
//    printf("make_persistant_taskId: last_write: %d", new_ni->last_write);
   return new_ni;
}


/* find the node in the collection that records the last write
 * inside this address                                            */
static rb_red_blk_node* find_previous_write_record(t_Addr addr) {
   return RBExactQuery (all_dependent_memory_references, &addr);
}


/* add write record [addr, ni] to the collection    */
static void record_write (t_Addr addr, NodeInfo* ni) {
   t_Addr *newAddr;
   NodeInfo *newNodeInfo;
   
#if library_TESTING
   /* make sure there is NOT already present taskname  */
   rb_red_blk_node *newNode;
   newNode = RBExactQuery( all_dependent_memory_references, &addr);
   if (newNode != NULL) 
      PANIC("duplication of addresses in the dependency tree, repeated Address: new->%p and old->%p\n",
            addr,  *(t_Addr *)(newNode->key));
#endif   
   
   /* make space for storing taskname and taskcode  */
   newAddr  	= make_persistant_addr(addr);
   newNodeInfo  = make_persistant_taskId(ni);   
     
   /* insert to the tree  */   
   RBTreeInsert(all_dependent_memory_references,
                newAddr, newNodeInfo);   
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
t_taskId* mark_input (t_taskId actual_task, t_Addr addr, int* n_depending_tasks) {
   int i;
   NodeInfo* NI = NULL;
   NI= (NodeInfo*) malloc(sizeof(NodeInfo));
   NI->last_write= no_dependency_task;
   //NI->array_size= 0;
   NI->array_ptr= NULL;
   rb_red_blk_node *previous_write_taskID_node;
   assert(libraryStatus == initialized);        
   previous_write_taskID_node = find_previous_write_record (addr);
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      NI = ((NodeInfo*) RBTreeGetInfo(all_dependent_memory_references, previous_write_taskID_node));
   }
   if(NI->array_ptr) {
      *n_depending_tasks = NI->spaces_occupied;
      if (!array_aux) array_aux= malloc(NI->spaces_occupied * sizeof(t_taskId));
      if ((sizeof(array_aux)) < (NI->spaces_occupied * sizeof(t_taskId))) 
	 array_aux= realloc(array_aux, (NI->spaces_occupied * sizeof(t_taskId)));
      for (i=0; i<NI->spaces_occupied; ++i) array_aux[i] = *(NI->array_ptr + i);
      free(NI->array_ptr);
      NI->array_ptr = NULL;
      RBTreeSetInfo(all_dependent_memory_references, previous_write_taskID_node, NI);
      return &array_aux[0];
   }
   return (t_taskId*)&NI->last_write;
}

/* mark output access */
t_taskId mark_output (t_taskId actual_task, t_Addr addr) {
   NodeInfo* NI = NULL;
   NI= (NodeInfo*) malloc(sizeof(NodeInfo));
   rb_red_blk_node *previous_write_taskID_node;
   assert(libraryStatus == initialized);     
   
   /* update the last task that has writen to this memory reference */
   previous_write_taskID_node = find_previous_write_record (addr);
   NI->last_write = actual_task;
   NI->array_size= 0;
   NI->spaces_occupied= 0;
   NI->array_ptr= NULL;
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      RBTreeSetInfo(all_dependent_memory_references,
                    previous_write_taskID_node,
                    NI);
   } else {
      record_write (addr, NI);
   }
   
   /* return no dependency, because of buffer renaming */
   return no_dependency_task;
}



/* mark inout access */
t_taskId* mark_inout (t_taskId actual_task, t_Addr addr, int* n_depending_tasks) {
   int i;
   NodeInfo* NI = NULL;		//acutal Node_Info
   NodeInfo* NIaux = NULL; 	//auxiliar NodeInfo
   NI= (NodeInfo*) malloc(sizeof(NodeInfo));
   NIaux= (NodeInfo*) malloc(sizeof(NodeInfo));
   rb_red_blk_node *previous_write_taskID_node;
   assert(libraryStatus == initialized);
   /* assume there will be no dependency */
   NIaux->last_write = no_dependency_task;
   NIaux->array_ptr = NULL;
   /* find if there is dependency from some previous task */
   previous_write_taskID_node = find_previous_write_record (addr);
   
   if (previous_write_taskID_node != NO_NODE_FOUND) {
      /* Get the node that contains the data with address addr */
      NI =  ((NodeInfo*) RBTreeGetInfo(all_dependent_memory_references, previous_write_taskID_node));
      
      /* Copy the data needed at NIaux */
      if (!NI->array_ptr) NIaux->last_write = NI->last_write; 
      else {
	 NIaux->array_ptr = NI->array_ptr;
	 NIaux->spaces_occupied = NI->spaces_occupied;
      }
      
      /* Last write is now acual task */
      NI->last_write = actual_task;
      /* Set the changes */
      RBTreeSetInfo(all_dependent_memory_references, previous_write_taskID_node, NI);
   } else {
      /* make new record in the collection */
      /* and there is no dependency to report */      
      NI->last_write = actual_task;
      NI->array_size= 0;
      NI->spaces_occupied= 0;
      NI->array_ptr= NULL;
      record_write (addr, NI);
   }   
   /* return the caught dependency*/
   if (NIaux->array_ptr){
      *n_depending_tasks = NIaux->spaces_occupied;
      if (!array_aux) array_aux= malloc(NIaux->spaces_occupied * sizeof(t_taskId));
      else if ((sizeof(array_aux)) < (NIaux->spaces_occupied * sizeof(t_taskId))) 
	 array_aux= realloc(array_aux, (NIaux->spaces_occupied * sizeof(t_taskId)));
      for (i=0; i<NIaux->spaces_occupied; ++i) array_aux[i] = *(NIaux->array_ptr + i);
      free(NIaux->array_ptr);
      NIaux->array_ptr = NULL;
      RBTreeSetInfo(all_dependent_memory_references, previous_write_taskID_node, NI);
      return &array_aux[0];
      
      *n_depending_tasks = NIaux->spaces_occupied;
      return NIaux->array_ptr;
   }
   return (t_taskId*)&NIaux->last_write;
}


/* mark commutative access */
t_taskId* mark_commutative (t_taskId actual_task, t_Addr addr, int* n_depending_tasks) {
   NodeInfo* NI = NULL;
   NI= (NodeInfo*) malloc(sizeof(NodeInfo));
   NI->last_write= no_dependency_task;
   NI->array_size= 0;
   NI->spaces_occupied= 0;
   NI->array_ptr= NULL;
   
   rb_red_blk_node *previous_write_taskID_node;
   assert(libraryStatus == initialized);        
   
   previous_write_taskID_node = find_previous_write_record (addr);
   if (previous_write_taskID_node == NO_NODE_FOUND) {
      record_write (addr, NI);
      previous_write_taskID_node = find_previous_write_record (addr);
   }
   assert(previous_write_taskID_node != NO_NODE_FOUND);
   
   NI = ((NodeInfo*) RBTreeGetInfo(all_dependent_memory_references, previous_write_taskID_node));
   
   /* actualize array->ptr */
   if (!NI->array_ptr) {
      NI->array_ptr = malloc (10*sizeof(NodeInfo));
      NI->array_size = 10;
   }
   else if (NI->array_size == NI->spaces_occupied) {
      NI->array_ptr = realloc(NI->array_ptr, 2*NI->array_size*sizeof(t_taskId));
      NI->array_size *= 2;
   }
   *(NI->array_ptr + NI->spaces_occupied) = actual_task;
   ++NI->spaces_occupied;
   
   RBTreeSetInfo(all_dependent_memory_references, previous_write_taskID_node, NI);
   
   return (t_taskId*)&NI->last_write;
}

void printout_all_dependencies_collection (void) {
   assert(libraryStatus == initialized);   
   printf("collection of dependencies \n");
   RBTreePrint(all_dependent_memory_references);
   printf("\n");
}





