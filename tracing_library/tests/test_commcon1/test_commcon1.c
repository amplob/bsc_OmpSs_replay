#include <stdio.h>

#define SIZE1 10000
#define SIZE2 SIZE1*10

#pragma omp task inout(X[0;10000]) no_copy_deps
void some_work_inout(int *X) {
	int i;
	for (i=0; i<SIZE2; ++i) X[i]+=i;
	printf("finish some_work_inout\n");
}

#pragma omp task concurrent (X[0;10000]) no_copy_deps
void some_work_conc(int *X, int part) {
	int i;
	for (i=0; i<SIZE1; ++i) 
	    X[part * SIZE1 + i]+=i;
	printf("finish some_work_concurrent, values from %d to %d\n", part * SIZE1, part * SIZE1 +999);
}

#pragma omp task commutative (X[0;10000]) no_copy_deps
void some_work_comm(int *X, int part) {
	int i;
	for (i=0; i<SIZE1; ++i) 
	    X[part * SIZE1 + i]+=i;
	printf("finish some_work_commutative, values from %d to %d\n", part * SIZE1, part * SIZE1 +999);
}



int main(int argc, char *argv[])
{
      int X[SIZE2];
      int Y[SIZE2];
      
      int i;
      char *task_name = "task number 1";
      
      printf("starting program\n");
      //Extrae_init();
      
      for (i=0; i<2; i++) 
      {
	some_work_conc(X, i);
      }
      #pragma omp taskwait
      for (i=0; i<2; i++) 
      {
	some_work_comm(X, i);
      }
      #pragma omp taskwait
      some_work_inout(X);
      
      
      for (i=0; i<2; i++) 
      {
	some_work_comm(Y, i);
      }
      #pragma omp taskwait
      for (i=0; i<2; i++) 
      {
	some_work_conc(Y, i);
      }
      #pragma omp taskwait

      some_work_inout(Y); 
      
      
      printf("program end\n");
      #pragma omp taskwait
      
      
      return 0;
      //Extrae_fini();
      //return 0;
}













