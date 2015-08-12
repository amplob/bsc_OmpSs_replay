
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
	Extrae_event( 1000, 1);
// 	  printf("starting task a\n");
          some_work(X, 'a');
// 	  printf("ending task a\n");
	  Extrae_event( 1000, 0);
      }
#pragma omp task in(a) out(b)
      {
	Extrae_event( 1000, 2);
          some_work(X, 'b');
	  Extrae_event( 1000, 0);
      }
#pragma omp task in(a) out (c)
      {
	Extrae_event( 1000, 3);
          some_work(X, 'c');
	  Extrae_event( 1000, 0);
      }    
#pragma omp task in(c) out (d)
      {
	Extrae_event( 1000, 4);
          some_work(X, 'd');
	  Extrae_event( 1000, 0);
      }
#pragma omp task in(d) out (e)
      {
	Extrae_event( 1000, 5);
          some_work(X, 'e');
	  Extrae_event( 1000, 0);
      }
#pragma omp task in(d) out (f)
      {
	Extrae_event( 1000, 6);
          some_work(X, 'f');
	  Extrae_event( 1000, 0);
      }
#pragma omp task in(b) in (e) in (f)
      {
	Extrae_event( 1000, 7);
          some_work(X, 'g');
	  Extrae_event( 1000, 0);
      }

      Extrae_fini();
#pragma omp taskwait
      return 0;
}



















