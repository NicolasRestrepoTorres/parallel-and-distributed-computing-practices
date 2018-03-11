#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static long num_steps = 100000;
double step, pi;
int THREADS;


int main(int argc, char* argv[] ){
	
	THREADS = atoi(argv[1]);
	int nthreads;
	
	step = 1.0/(double) num_steps; 
	
	double start_t = omp_get_wtime();
	
	omp_set_num_threads(THREADS);

	
	pi = 0.0; 
	#pragma omp for reduction(+:pi)
	for (int i=0; i< num_steps; i++){
		pi += (4.0/(1.0+(i+0.5)*step*(i+0.5)*step))*step;	
	}


	double end_t = omp_get_wtime();
	
	printf("pi: %f\n", pi);
	printf("time: %f\n", end_t - start_t);
	return 0;
}
