#include <stdio.h>

int a;

#pragma omp task in (X[100000])
void some_work1(int *X, char task);

#pragma omp task out (X[100000])
void some_work2(int *X, char task);


void some_work1(int *X, char task) {
	int i;
	for (i=0; i<100000; ++i) X[i]=i;
	printf("Done task %c\n", task);
}

void some_work2(int *X, char task) {
	int i;
	for (i=0; i<100000; ++i) X[i]=i;
	printf("Done task %c\n", task);
}


int main(int argc, char *argv[])
{
      int x = argc;
      int X[100000];
      int its = 10;
      int i;
      
      printf("starting program\n");
      Extrae_init();
      
      //requires variable a (that may be modified) and after that it's readed
      for (i=0; i<its; ++i) {
	  some_work2(X, 'a');
	  some_work1(X, 'a');
// 	  #pragma omp barrier
      }
//       #pragma omp taskwait
      
      //variable a is only written
      for (i=0; i<its; ++i) {
	  some_work2(X, 'a');
	  some_work2(X, 'a');
// 	  #pragma omp barrier
      }
//       #pragma omp taskwait
      
      //variable a is only readed
      for (i=0; i<its; ++i) {
	  some_work1(X, 'a');
	  some_work1(X, 'a');
// 	  #pragma omp barrier
      }
//       #pragma omp taskwait
      
      
      //reads variable a and is written afterwards
      for (i=0; i<its; ++i) {
	  some_work1(X, 'a');
	  some_work2(X, 'a');
	  
      }
//       #pragma omp taskwait
      
      Extrae_fini();
      return 0;
}



















