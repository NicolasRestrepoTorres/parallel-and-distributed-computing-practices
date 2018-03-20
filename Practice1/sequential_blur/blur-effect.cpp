#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>

#define ITERATIONS 1
using namespace cv;
using namespace std;
Mat img, blurred;
int k;


bool checkBounds(int a, int b, int bx, int by){
	if(a<0 || a>=bx) return false;
	if(b<0 || b>=by) return false;
	return true;
}

void BoxBlur(){
	int accumulator, cornerX, cornerY, sumRGB[] = {0,0,0};
	//printf("Hilo: %d\n", id);
	for(int x=0;x<img.cols;x++){
		//printf("hilo: %d, col: %d\n", id, x);
		for(int y=0;y<img.rows;y++){
			//sem_wait(&semvar2);
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
				sumRGB[0] = img.at<Vec3b>(Point(x,y))(0);
				sumRGB[1] = img.at<Vec3b>(Point(x,y))(1);
				sumRGB[2] = img.at<Vec3b>(Point(x,y))(2);
			}else{
				sumRGB[0] = sumRGB[0] / accumulator;
				sumRGB[1] = sumRGB[1] / accumulator;
				sumRGB[2] = sumRGB[2] / accumulator;
			}

			Vec3b color;
			color(0) = sumRGB[0];
			color(1) = sumRGB[1];
			color(2) = sumRGB[2];
			blurred.at<Vec3b>(Point(x,y)) = color;


		}
	}
}

int main(int argc, char** argv ){
		clock_t start, end;

    start=clock();

    img = imread( argv[1], 1 );
		if ( !img.data ){
        printf("No image data \n");
        return -1;
    }

		k = atoi(argv[3]);
		blurred = Mat(img.rows, img.cols, CV_8UC3, Scalar(255,255,255));
		BoxBlur();


		imwrite( argv[2], blurred );

		end=clock();
		double time_taken = (double) (end-start)/CLOCKS_PER_SEC;

		// cout << "The image " << argv[1] << " with " << img.rows << " rows and "
		//  << img.cols << " cols was blurred with " << ITERATIONS <<
		//   " iterations and a kernel size of " <<  k << " in "
		// 	<< time_taken << " seconds."<< endl;
			// cout << "Image \"" << argv[1] << "\":" << endl;
			// printf("Rows: %d Cols: %d\n", img.rows, img.cols);
			// printf("Kernel Size: %d \n", k);
			// printf("Iterations: %d \n", ITERATIONS);
			// printf("Time: %f seconds\n", time_taken);
			// cout << endl << endl;

			printf("%f\n", time_taken);
		return 0;
}
