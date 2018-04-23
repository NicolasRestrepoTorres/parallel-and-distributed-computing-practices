#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static int num_steps=21000;
double e;
int THREADS;

/*
unsigned long long factorial(int k){
  unsigned long long fact = k;
  for(int i=k-1; i>1; i--){
    fact *= i;
  }
  printf("fact %d: %llu\n", k, fact);
  return fact;
}*/

int main(int argc, char* argv[] ){

	THREADS = atoi(argv[1]);
	int nthreads, f;
  unsigned long long fact[num_steps];

  fact[0]=1;
  for(f=1; f<num_steps; f++){
    fact[f] = fact[f-1]*f;
  }

	double start_t = omp_get_wtime();

	omp_set_num_threads(THREADS);
  nthreads = omp_get_num_threads();

	e = 0.0;
  long int z;
	#pragma omp for reduction(+:e)
	for (int k=1; k< num_steps; k++){
    if (k == 1){
      z = 1;
    }else {
     float e = 2.71;
     z = sqrt(2*3.14*k) * pow((k/e), k);
   }
    e += k*0.5/z;
	}


	double end_t = omp_get_wtime();

	printf("e: %f\n", e);
	printf("time: %f\n", end_t - start_t);
	return 0;
}
