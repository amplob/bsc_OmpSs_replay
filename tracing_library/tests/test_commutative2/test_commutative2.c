#include <stdio.h>

#include "/home/bsc/bsc/tools/OmpSs_replay/tests-utils/print.h"

void some_work(int *X);

#define SIZE1 1000
#define SIZE2 SIZE1*10

#pragma omp task inout(X[0;10000]) no_copy_deps
void some_work(int *X) {
	int i;
	for (i=0; i<SIZE2; ++i) X[i]+=i;
	printf("finish some work\n");
}

#pragma omp task commutative (X[0;10000]) no_copy_deps
void some_work2(int *X, int part) {
	int i;
	for (i=0; i<SIZE1; ++i) 
	    X[part * SIZE1 + i]+=i;
	printf("finish some work2, values from %d to %d\n", part * SIZE1, part * SIZE1 +(SIZE1-1));
}

#pragma omp task commutative (Y[0;10000]) no_copy_deps
void some_work3(int *Y, int part) {
	int i;
	for (i=0; i<SIZE1; ++i) 
	    Y[part * SIZE1 + i]+=i;
	printf("finish some work3, values from %d to %d\n", part * SIZE1, part * SIZE1 +(SIZE1-1));
}


int main(int argc, char *argv[])
{
      int X[SIZE2];
      int Y[SIZE2];
      
      int i;
      char *task_name = "task number 1";
      
      printf("starting program\n");
      Extrae_init();

      some_work(X); 
      some_work(Y); 
      
      for (i=0; i<2; i++) 
      {
	some_work2(X, i);
      }

      for (i=0; i<2; i++) 
      {
	some_work3(Y, i);
      }

      for (i=0; i<2; i++) 
      {
	some_work2(X, i);
      }
      #pragma omp taskwait

      some_work(X);
      some_work(Y); 
      
      printf("program end\n");
      #pragma omp taskwait
      
      return 0;
      Extrae_fini();
      //return 0;
}













