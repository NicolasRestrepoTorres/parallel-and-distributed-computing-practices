#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ITERATIONS 1
using namespace cv;
using namespace std;
struct timeval  tv1, tv2;
int THREADS, k;
sem_t semvar, semvar2;
Mat img, blurred;

bool checkBounds(int a, int b, int bx, int by){
	if(a<0 || a>bx) return false;
	if(b<0 || b>by) return false;
	return true;
}

int *getAverageAround(int a, int b){

	int accumulator, cornerX, cornerY, sumRGB[] = {0,0,0};
	static int arr[3];
	accumulator=0;
	if(k%2==0){
		cornerX = a-(k-1)/2;
		cornerY = b-(k-1)/2;
	}else{
		cornerX = a-(k-2)/2;
		cornerY = b-(k-2)/2;
	}

	for(int i = 0; i < k; i++){
		for(int j = 0; j < k; j++){
			if(checkBounds(cornerX+j,cornerY+i, img.cols, img.rows)){
				Vec3b color = img.at<Vec3b>(Point(cornerX+j,cornerY+i));
				sumRGB[0] += (int) color(0);
				sumRGB[1] += (int) color(1);
				sumRGB[2] += (int) color(2);
				accumulator++;
			}
		}
	}

	if(accumulator == 0){
		arr[0] = img.at<Vec3b>(Point(a,b))(0);
		arr[1] = img.at<Vec3b>(Point(a,b))(1);
		arr[2] = img.at<Vec3b>(Point(a,b))(2);
	}else{
		arr[0] = sumRGB[0] / accumulator;
		arr[1] = sumRGB[1] / accumulator;
		arr[2] = sumRGB[2] / accumulator;
	}
	return arr;
}


void *BoxBlur(void*ap){
	int id;
	id = *((int *) ap);
	id--;

	for(int x=id;x<img.cols;x+=THREADS){
		for(int y=0;y<img.rows;y++){

			int *results = getAverageAround(x, y);

			Vec3b color;
			color(0) = results[0];
			color(1) = results[1];
			color(2) = results[2];
			sem_wait(&semvar);
				blurred.at<Vec3b>(Point(x,y)) = color;
			sem_post(&semvar);

		}
	}
}

int main(int argc, char** argv ){
		clock_t start, end;
		sem_init(&semvar, 0, 1);
		sem_init(&semvar2, 0, 1);
		int r, t;

		start=clock();
		gettimeofday(&tv1, NULL);

    img = imread( argv[1], 1 );
		if ( !img.data ){
        printf("No image data \n");
        return -1;
    }

		k = atoi(argv[3]);
		THREADS = atoi(argv[4]);
		pthread_t hilo[THREADS];
		blurred = Mat(img.rows, img.cols, CV_8UC3, Scalar(255,255,255));

		for(int t=0; t<THREADS; t++){
			r = pthread_create(&hilo[t], NULL, &BoxBlur, (void*)&t);
			if(r != 0){
				perror("Error en pthread_create");
				exit(-1);
			}
		}

		for(int t=0; t<THREADS; t++){
			r = pthread_join(hilo[t], NULL);
			if(r != 0){
				perror("Error en pthread_join");
				exit(-1);
			}
		}



		imwrite( argv[2], blurred );

		end=clock();
		double time_taken = (double) (end-start)/CLOCKS_PER_SEC;
		gettimeofday(&tv2, NULL);

		cout << "The image " << argv[1] << " with " << img.rows << " rows and " << img.cols << " cols was blurred with " << ITERATIONS << " iterations in " << time_taken << " seconds."<< endl;

		printf("Total time = %f seconds\n",
	         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec));

		sem_destroy(&semvar);
		return 0;
}
