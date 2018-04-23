#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

using namespace cv;
using namespace std;
struct timeval  tv1, tv2;
Mat img, blurred;
//Compile with nvcc cudaBlur.cu `pkg-config opencv --cflags --libs`

__global__ void BoxBlur(uchar4 *d_img, uchar4 *d_blurred, int numRows, int numCols, int k){

	int id = threadIdx.x;
	int accumulator, cornerX, cornerY, sumRGB[] = {0,0,0};

		printf("Hilo: %d\n", id);
		for(int x=id; x<numRows; x+=blockDim.x){
			//printf("hilo: %d, col: %d\n", id, x);
			for(int y=0; y<numCols; y++){
				//sem_wait(&semvar2);
				//cout << y << endl;
        int pixel = numCols * x + y;
        uchar4 pixelColor = d_img[pixel];
				sumRGB[0]=0;
				sumRGB[1]=0;
				sumRGB[2]=0;
				accumulator=0;
				if(k%2==0){
					cornerX = x-(k-1)/2;
					cornerY = y-(k-1)/2;
				}else{
					cornerX = x-(k-2)/2;
					cornerY = y-(k-2)/2;
				}

				for(int i = 0; i < k; i++){
					for(int j = 0; j < k; j++){
            if(cornerX+i>=0 && cornerX+i<numRows && cornerY+j>=0 && cornerY+j<numCols) {
              int index = numCols * (cornerX+i) + (cornerY+j);
							uchar4 color = d_img[index];
							sumRGB[0] += (int) color.x;
							sumRGB[1] += (int) color.y;
							sumRGB[2] += (int) color.z;
							accumulator++;
						}
					}
				}



				if(accumulator == 0){
					sumRGB[0] = pixelColor.x;
					sumRGB[1] = pixelColor.y;
					sumRGB[2] = pixelColor.z;
				}else{
					sumRGB[0] = sumRGB[0] / accumulator;
					sumRGB[1] = sumRGB[1] / accumulator;
					sumRGB[2] = sumRGB[2] / accumulator;
				}

				uchar4 color;
				color.x = sumRGB[0];
				color.y = sumRGB[1];
				color.z = sumRGB[2];


				d_blurred[pixel] = color;


			}
		}


}

int main(int argc, char** argv ){

    printf("-2");

    img = imread( argv[1], 1 );
		if ( !img.data ){
        printf("No image data \n");
        return -1;
    }

    printf("-1");
    size_t numRows =  img.rows;
    size_t numCols = img.cols;
    const size_t numPixels = numRows * numCols;

		size_t k = atoi(argv[3]);
		int THREADS = atoi(argv[4]);
    printf("0");

    uchar4 *h_img = img.ptr<uchar4>(0);
    uchar4 *h_blurred = blurred.ptr<uchar4>(0);
    uchar4 *d_img;
    uchar4 *d_blurred;
    printf("1");

    //allocate memory on the device for both input and output
    cudaMalloc(&d_img, sizeof(uchar4) * numPixels);
    cudaMalloc(&d_blurred, sizeof(uchar4) * numPixels);
    printf("2");

    //make sure no memory is left laying around
    cudaMemset(d_blurred, 0, numPixels * sizeof(uchar4));
    printf("3");
    //copy input array to the GPU
    cudaMemcpy(&d_img, &h_img, sizeof(uchar4) * numPixels, cudaMemcpyHostToDevice);
    printf("4");
		BoxBlur<<<1, THREADS>>>(d_img, d_blurred, numRows, numCols, k);
    printf("5");
    blurred = Mat(numRows, numCols, CV_8UC3, (void*)&h_blurred);

		imwrite( argv[2], blurred );

		return 0;
}
