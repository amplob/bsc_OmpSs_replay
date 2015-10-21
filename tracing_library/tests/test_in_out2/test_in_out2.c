#include <stdio.h>



void some_work(int *aX, char task);

void some_work(int *X, char task) {
	int i;
	for (i=0; i<300000; ++i) X[i]=i;
 	printf("Done task %c\n", task);
}

int main(int argc, char *argv[])
{
      int x = argc;
      int aX[300000];
      int bX[300000];
      int i;
      
      printf("starting program\n");
      Extrae_init();
      
//       for (i=0; i<1; ++i) {
	  #pragma omp task inout (aX)
	  some_work(aX, 'a');
	  
	  #pragma omp task inout (bX)
	  some_work(bX, 'b');
//       }
      
      Extrae_fini();
      return 0;
}



















