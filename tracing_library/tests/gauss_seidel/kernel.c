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

#pragma omp task in([BS]top, [BS]left, [BS]bottom, [BS]right) inout([BS]A)
void gauss_seidel(int first, int second, long N, long BS, double A[BS][BS], double top[BS][BS], double left[BS][BS], double bottom[BS][BS], double right[BS][BS]) {
	// EXTRAE ADD EVENT (TYPE0, 10*i+j)        
	for (long i=0; i < BS; i++) {
		for (long j=0; j < BS; j++) {
			A[i][j] = 0.2 * (
				A[i][j]
				+ (i > 0L ? A[i-1][j] : top[BS-1][j])
				+ (i < BS-1L ? A[i+1][j] : bottom[0][j])
				+ (j > 0L ? A[i][j-1] : left[i][BS-1])
				+ (j < BS-1L ? A[i][j+1] : right[i][0])
			);
		}
	}
// 	printf("Thread %d have done task (%d-%d, %d-%d)\n", omp_get_thread_num(), top, bottom, left, right); 
//  	printf("done task (%d-%d)\n", first, second);
}
