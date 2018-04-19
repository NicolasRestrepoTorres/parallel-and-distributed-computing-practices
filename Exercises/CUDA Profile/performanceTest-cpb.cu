/*programa para hacer un calculo intensivo y hacer nvprof*/

#include <stdio.h>

#include <cuda_runtime.h>
#define ITERATIONS 1E07

__global__ void calcSin(float *A)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    int i;
    double acc, ang = 10.0;
      for(i = 0; i < ITERATIONS; i++){
        ang = i;
        acc = sin(ang);
      }
      *A = acc;
}

__global__ void calcAdd(float *A)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    int i;
    double acc, ang = 10.0;
      for(i = 0; i < ITERATIONS; i++){
        acc = acc + ang;
      }
      *A = acc;
}

__global__ void calcMul(float *A)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    int i;
    double acc = 1.0, ang = 13.0;
      for(i = 0; i < ITERATIONS; i++){
        acc = acc * i;
      }
      *A = acc;
}

int main(void)
{
    cudaError_t err = cudaSuccess;

    printf("Calculating...");
    float *d_A = NULL;
    float A;
    err = cudaMalloc((void **)&d_A, sizeof(float));
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    int threadsPerBlock = 48;
    int blocksPerGrid = 2;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    calcSin<<<blocksPerGrid, threadsPerBlock>>>(d_A);
    calcAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A);
    calcMul<<<blocksPerGrid, threadsPerBlock>>>(d_A);
	cudaDeviceSynchronize();
    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(&A, d_A, sizeof(float), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("\n%f ", A);
    err = cudaFree(d_A);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaDeviceReset();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to deinitialize the device! error=%s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("Done\n");
    return 0;
}
