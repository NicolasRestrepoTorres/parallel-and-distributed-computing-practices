#include <stdio.h>

__global__ void square( int *d_num_steps, unsigned long long *d_fact, double *d_out){
  int idx = threadIdx.x;
  int num_steps = *d_num_steps;
  for(int k=idx+1; k< num_steps; k+=blockDim.x){
    d_out[idx] += (double) k*0.5/ (double) d_fact[k-1];
  }

}

int main(int argc, char ** argv){
  int h_num_steps = 21;
  int THREADS = atoi(argv[1]);
  double e;

  // generate the output array on the host
  unsigned long long h_fact[h_num_steps];
  h_fact[0]=1;
  for(int f=1; f<h_num_steps; f++){
    h_fact[f] = h_fact[f-1]*f;
  }
  double h_out[THREADS];

  //declare GPU memory pointers
  int *d_num_steps;
  unsigned long long *d_fact;
  double *d_out;

  //allocate GPU memory
  cudaMalloc((void **) &d_num_steps, sizeof(int));
  cudaMalloc((void **) &d_fact, h_num_steps*sizeof(unsigned long long));
  cudaMalloc((void **) &d_out, THREADS*sizeof(double));

  // transfer the array to the GPU
  cudaMemcpy(d_num_steps, &h_num_steps, sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(d_fact, h_fact, h_num_steps*sizeof(unsigned long long), cudaMemcpyHostToDevice);

  // launch the kernel
  square<<<1, THREADS>>>(d_num_steps, d_fact, d_out);

  //copy back the result array to the CPU
  cudaMemcpy(h_out, d_out, THREADS*sizeof(double), cudaMemcpyDeviceToHost);

  //print out the resulting array
  for (int i=0; i<THREADS; i++){
    e+=h_out[i];
  }

  printf("e: %f\n", e);

  // free GPU memory allocation
  cudaFree(d_num_steps);
  cudaFree(d_fact);
  cudaFree(d_out);

  return 0;
}
