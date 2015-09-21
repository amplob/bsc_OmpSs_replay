
#include <stdio.h>


void some_work(int *X, char task) {
	int i;
	for (i=0; i<100000; ++i) X[i]=i;
	
	printf("Thread %d have done task %c\n", omp_get_thread_num(), task);
	
}


int main(int argc, char *argv[])
{
      int x = argc;
      int X[100000];
      
      int a, b, c, d, e, f, g;
      
      printf("starting program\n");
      Extrae_init();

#pragma omp task out(a)
      {
          some_work(X, 'a');
      }
      
#pragma omp task in(a) out(b)
      {
          some_work(X, 'b');
      }
      
#pragma omp task in(a) out (c)
      {
          some_work(X, 'c');
      }    
      
#pragma omp task in(c) out (d)
      {
          some_work(X, 'd');
      }
      
#pragma omp task in(d) out (e)
      {
          some_work(X, 'e');
      }
      
#pragma omp task in(d) out (f)
      {
          some_work(X, 'f');
      }
      
#pragma omp task in(b) in (e) in (f)
      {
          some_work(X, 'g');
      }

      Extrae_fini();
#pragma omp taskwait
      return 0;
}



















