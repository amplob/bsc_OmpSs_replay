#ifndef GLOBAL_H

#include <assert.h>
#include "rb_tree/red_black_tree.h"
#include "Config.h"
#define MAX_TASKNAME_LEN  128

#if PARALLEL_LIBRARY
#include "mpi.h"
#endif

#if library_TESTING
   #define TEST_PROGRESS(x...) \
            {                  \
               printf("Testing: "); printf(x);      \
            }
#else
   #define TEST_PROGRESS(x...)  
#endif

typedef unsigned int t_taskcode;
typedef void * t_Addr;
typedef int t_taskId;
typedef unsigned int bool;
typedef enum e_LibraryStatus { initialized, uninitialized } LibraryStatus;
typedef enum e_ActualTaskType { NoTask, In, Out, InOut, Commutative} ActualTaskType;
typedef int t_phaseID;

#define NO_NODE_FOUND NULL
/* const rb_red_blk_node *noNode = (rb_red_blk_node*) NULL;   */

#define PANIC(x...)    \
        {   \
           printf(x);  \
           assert(0);    \
        }
        
        
#define True 1
#define False 0


#define GLOBAL_H
#endif
