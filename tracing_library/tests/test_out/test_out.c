#include <stdio.h>

int a;

#pragma omp task out (a)
void some_work(int *X, char task);


void some_work(int *X, char task) {
	int i;
	for (i=0; i<100000; ++i) X[i]=i;
// 	printf("Thread %d have done task %c\n", omp_get_thread_num(), task);
}


int main(int argc, char *argv[])
{
      int x = argc;
      int X[100000];
      
      int i;
      
      printf("starting program\n");
      Extrae_init();
      
      for (i=0; i<1000; ++i) {
	  some_work(X, 'a');
      }
      
      Extrae_fini();
      return 0;
}



















