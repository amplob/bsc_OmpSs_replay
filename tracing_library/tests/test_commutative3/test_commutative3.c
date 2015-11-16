#include <stdio.h>


#pragma omp task inout(X[0;10000]) no_copy_deps
void some_work_inout(int *X) {
	int i;
	for (i=0; i<10000; ++i) X[i]+=i;
	printf("finish some_work_inout\n");
}

#pragma omp task commutative (X[0;10000]) no_copy_deps
void some_work_comm(int *X, int part) {
	int i;
	for (i=0; i<1000; ++i) 
	    X[part * 1000 + i]+=i;
	printf("finish some_work_comm, values from %d to %d\n", part * 1000, part * 1000 +999);
}



int main(int argc, char *argv[])
{
      int X[10000];
      int Y[10000];
      
      int i;
      char *task_name = "task number 1";
      
      printf("starting program\n");
      //Extrae_init();
      
      for (i=0; i<2; i++) 
      {
	some_work_comm(X, i);
      }

      for (i=0; i<2; i++) 
      {
	some_work_comm(Y, i);
      }
      #pragma omp taskwait
      some_work_inout(Y);

      for (i=0; i<2; i++) 
      {
	some_work_comm(X, i);
      }
      #pragma omp taskwait

      some_work_inout(X); 
      
      printf("program end\n");
      #pragma omp taskwait
      
      return 0;
      //Extrae_fini();
      //return 0;
}













