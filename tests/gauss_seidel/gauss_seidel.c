/*
 * Copyright (c) 2008, BSC (Barcelona Supercomputing Center)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BSC ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h> 


#define TYPE double
#include "/home/bsc/bsc/tools/OmpSs_replay/tests-utils/init.h"
#include "/home/bsc/bsc/tools/OmpSs_replay/tests-utils/timer.h"

#define OVERRIDE_MALLOC_AND_FREE
#include "memory.h"


static inline double initializeElement(long I, long J, long i, long j) {
	return ((i+I)*(j+J)) % 1325;
}


static double *alloc_and_gen_matrix(long N, long BS) {
	typedef double (*m_t)[N/BS][BS][BS];
	
	m_t A = (m_t) malloc(sizeof(double)*N*N);
	
	if (*A == NULL) {
		fprintf(stderr, "Memory allocation error %iMB\n", (sizeof(double)*N*N)/(1024L*1024L));
		exit(1);
	}
	
	
	initializeUsingUserSpecification(N, N, BS, BS, A);

	for (int i=0; i<N/BS; ++i) {
	  for (int j=0; j<N/BS; ++j) {
	    for (int k=0; k<BS; ++k) {
	      for (int l=0; l<BS; ++l) {
		A[i][j][k][l]= 4.0 * i;
// 		printf (" %f", A[i][j][k][l]);
	      }
	    }
// 	    printf("\n");
	  }
	}
	
	return (double *)A;
}





#pragma omp task in([BS]top, [BS]left, [BS]bottom, [BS]right) inout([BS]A)
void gauss_seidel(int first, int second, long N, long BS, double A[BS][BS], double top[BS][BS], double left[BS][BS], double bottom[BS][BS], double right[BS][BS]);

void wrapperxxxxxxx(int first, int second, long N, long BS, double A[BS][BS], double top[BS][BS], double left[BS][BS], double bottom[BS][BS], double right[BS][BS]) 
{
  gauss_seidel(first, second, N, BS, A, top, left, bottom, right);
}


int main(int argc, char* argv[]) {
	if (argc != 7) {
		fprintf(stderr, "Usage: %s <N> <N> <block size> <block size> <index> <B|PW|SEQ>\n", argv[0]);
		return 1;
	}
	parseInitializationType(argv[6]);
	
	long N = atol(argv[1]);
	long BS = atol(argv[3]);
	// long L = atol(argv[4]);
	long L = 16;
	
	if (BS > N) {
		printf("0");
		return 1;
	}
	
	typedef double (*m_t)[N/BS + 2][BS][BS];
	
	m_t A = (m_t) alloc_and_gen_matrix(N + BS*2, BS);
	
	
	for (int i=0; i<N/BS; ++i) {
	  for (int j=0; j<N/BS; ++j) {
	    for (int k=0; k<BS; ++k) {
	      for (int l=0; l<BS; ++l) {
		//printf (" %f", A[i][j][k][l]);
	      }
	    }
// 	    printf ("\n");
	  }
	}
// 	printf ("\n");
	
	
	#pragma omp taskwait
	
	Extrae_init();
	printf("extrae init\n");
	START_TIME();
	for (int iters=0; iters<L; iters++) {
		for (long i=1; i <= N/BS; i++) {
			for (long j=1; j <= N/BS; j++) {
				wrapperxxxxxxx(i, j, (N+BS+BS), BS, A[i][j], A[i-1][j], A[i][j-1], A[i+1][j], A[i][j+1]);
			}
		}
	}
	printf("extrae fini\n");
	Extrae_fini();
  
	#pragma omp taskwait
	STOP_TIME();
	
	//#pragma omp critical 
	{
	for (int iters=0; iters<N; iters++) {
		for (int i=0; i <= N; i++) {
			//printf (" %f", *A[iters][i]);
		}
// 		printf ("\n");
	}
// 	printf ("\n");
	}
	
	if (getenv("MEASURE_TIME") == NULL) {
		// Megaelements per second
		printf("%f", ((double)(L*N*N))/GET_TIME());
	} else {
		printf("%f", GET_TIME() / 1000000.0);
	}
	
	return 0;
}

