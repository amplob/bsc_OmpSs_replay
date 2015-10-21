#include <stdio.h>



void some_work(int *X, char task);


void some_work(int *X, char task) {
	int i;
	for (i=0; i<300000; ++i) X[i]=i;
 	printf("Done task %c\n", task);
}


int main(int argc, char *argv[])
{
      int x = argc;
      int X[300000];
      int a,b,c,d,e,f,g,h;
      int i;
      
      printf("starting program\n");
      Extrae_init();
      
      for (i=0; i<3; ++i) {
	  #pragma omp task out (b,c) inout(a)
	  some_work(X, '1');
	  
	  #pragma omp task inout (b,c)
	  some_work(X, '2');
	  
	  #pragma omp task inout (b,c) out (a,d)
	  some_work(X, '3');
	  
	  #pragma omp task in (d)
	  some_work(X, '4');
	  
	  #pragma omp task in (d)
	  some_work(X, '5');
	  
	  #pragma omp task inout (a) in(d)
	  some_work(X, '6');
      }
      
      Extrae_fini();
      return 0;
}



















