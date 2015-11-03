
#include <stdio.h>

#pragma omp task
void some_work(int *X, char task);


void some_work(int *X, char task) {
	int i;
	for (i=0; i<100000; ++i) X[i]=i;
	printf("Done task %c\n", task);
}


int main(int argc, char *argv[])
{
      int x = argc;
      int X[100000];
      
      int i;
      
      printf("starting program\n");
      Extrae_init();
      
      for (i=0; i<100; ++i) {
	  some_work(X, 'a');
      }
      
      Extrae_fini();
      return 0;
}



















