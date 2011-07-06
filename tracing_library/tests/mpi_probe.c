#include "mpi.h"
#include "stdio.h"
#include <unistd.h>

#include "mpitrace_user_events.h"

int main(int argc, char *argv[]) {

   int my_rank;
  /* read options in the while loop
   */
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   MPI_Barrier (MPI_COMM_WORLD);
   MPItrace_event (1000, 2);
   sleep(2);
   MPItrace_event (1000, 0);
   MPI_Barrier (MPI_COMM_WORLD);   
   printf ("my rank is %d\n", my_rank);
   
   MPI_Finalize();
   
   return 0;
}
