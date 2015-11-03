#include <stdio.h>

#include "/home/bsc/bsc/tools/OmpSs_replay/tests-utils/print.h"

void some_work(int *X);

#pragma omp task inout(X[0;10000]) no_copy_deps
void some_work(int *X) {
	int i;
	for (i=0; i<10000; ++i) X[i]+=i;
	printf("finish some work\n");
}

#pragma omp task commutative (X[0;10000]) no_copy_deps
void some_work2(int *X, int part) {
	int i;
	for (i=0; i<1000; ++i) 
	    X[part * 1000 + i]+=i;
	printf("finish some work2, values from %d to %d\n", part * 1000, part * 1000 +999);
}



int main(int argc, char *argv[])
{
      int X[10000];
      int i;
      char *task_name = "task number 1";
      
      printf("starting program\n");
      //Extrae_init();

      some_work(X);      
      for (i=0; i<3; i++) 
      {
	some_work2(X, i);
      }
      #pragma omp taskwait
      
      some_work(X);
      
      printf("program end\n");
      #pragma omp taskwait
      
      return 0;
      //Extrae_fini();
      //return 0;
}













