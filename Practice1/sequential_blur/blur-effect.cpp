#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>

#define ITERATIONS 1
using namespace cv;
using namespace std;


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
			if(checkBounds(cornerX+j,cornerY+i, img.cols, img.rows)){
				Vec3b color = img.at<Vec3b>(Point(cornerX+j,cornerY+i));
				sumRGB[0] += (int) color(0);
				sumRGB[1] += (int) color(1);
				sumRGB[2] += (int) color(2);
				accumulator++;
			}
		}
	}

	arr[0] = sumRGB[0] / accumulator;
	arr[1] = sumRGB[1] / accumulator;
	arr[2] = sumRGB[2] / accumulator;

	if(accumulator == 0){
		arr[0] = img.at<Vec3b>(a,b)(0);
		arr[1] = img.at<Vec3b>(a,b)(1);
		arr[2] = img.at<Vec3b>(a,b)(2);
	}
	return arr;
}


Mat BoxBlur(Mat img, int k){
	Mat blurred = img;
	for(int x=0;x<img.cols;x++)
	{
		for(int y=0;y<img.rows;y++)
		{
			Vec3b color = img.at<Vec3b>(Point(x,y));
			int *results = getAverageAround(img, x, y, k);
			color(0) = results[0];
			color(1) = results[1];
			color(2) = results[2];
			img.at<Vec3b>(Point(x,y)) = color;
		}
	}
	return img;
}

int main(int argc, char** argv ){
		clock_t start, end;
		Mat img;
		int k;

    start=clock();

    img = imread( argv[1], 1 );
		if ( !img.data ){
        printf("No image data \n");
        return -1;
    }

		k = atoi(argv[3]);
		for(int i = 0; i < ITERATIONS; i++){
			BoxBlur(img, k);
		}

		imwrite( argv[2], img );

		end=clock();
		double time_taken = (double) (end-start)/CLOCKS_PER_SEC;

		cout << "The image " << argv[1] << " with " << img.rows << " rows and " << img.cols << " cols was blurred with " << ITERATIONS << " iterations in " << time_taken << " seconds."<< endl;

		return 0;
}
