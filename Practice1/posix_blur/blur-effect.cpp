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
int THREADS;
using namespace cv;
using namespace std;
struct timeval  tv1, tv2;
sem_t semvar;

struct thread_info{
	Mat img, blurred;
	int id;
	int k;
};

bool checkBounds(int a, int b, int bx, int by){
	if(a<0 || a>bx) return false;
	if(b<0 || b>by) return false;
	return true;
}

int *getAverageAround(Mat img, int a, int b, int k){

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
			if(checkBounds(cornerX+j,cornerY+i, img.rows, img.cols)){
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
	Mat blurred, img;
	int k, id;
	struct thread_info tinfo;
	tinfo = *(thread_info*)ap;

	for(int x=tinfo.id;x<img.cols;x+=THREADS)
	{
		for(int y=0;y<img.rows;y++)
		{
			Vec3b color = img.at<Vec3b>(Point(x,y));
			int *results = getAverageAround(tinfo.img, x, y, tinfo.k);
			color(0) = results[0];
			color(1) = results[1];
			color(2) = results[2];
			//sem_wait(&semvar);
				//printf("in\n");
				tinfo.blurred.at<Vec3b>(Point(x,y)) = color;
			//sem_post(&semvar);

		}
	}
	return NULL;
}

int main(int argc, char** argv ){
		clock_t start, end;
		Mat img, blurred;
		sem_init(&semvar, 0, 1);
		int k, r, i;
		struct thread_info tinfo;

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

		tinfo.img = img;
		tinfo.blurred = blurred = img.clone();
		tinfo.k = k;


		for(i = 0; i < ITERATIONS; i++){
			for(i=0; i<THREADS; i++){
				tinfo.id = i;
				r = pthread_create(&hilo[i], NULL, BoxBlur, (void*)&tinfo);
				if(r != 0){
					perror("Error en pthread_create");
					exit(-1);
				}
			}
		}

		for(i=0; i<THREADS; i++){
			r = pthread_join(hilo[i], NULL);
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
