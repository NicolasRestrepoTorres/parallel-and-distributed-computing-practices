#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define ITERATIONS 1
int THREADS;
using namespace cv;
using namespace std;
struct timeval  tv1, tv2;

struct thread_info{
	Mat img;
	int id;
};

bool checkBounds(int a, int b, int bx, int by){
	if(a<0 || a>bx) return false;
	if(b<0 || b>by) return false;
	return true;
}

int *getAverageAround(Mat img, int a, int b, int m){

	int tmp1 = 0, tmp2 = 0, sumR = 0,sumG = 0,sumB = 0, copya = a, copyb = b, x = 1;
	static int arr[3];
	int accumulatorR = 0, accumulatorG = 0, accumulatorB = 0;
	tmp1 = a-1;
	tmp2 = b-1;
	for(int i = 0; i < m*3; i++){
		a = copya; b = copyb;
		for(int j = 0; j < m*3; j++){
			if(checkBounds(tmp1+j,tmp2+i, img.cols, img.rows)){
				//cout << Point(tmp1,tmp2) << endl;
				Vec3b color = img.at<Vec3b>(Point(tmp1+j,tmp2+i));
				//cout << "Wichitig " << color << endl;
				sumR += (int) color(0);
				accumulatorR++;
				sumG += (int) color(1);
				accumulatorG++;
				sumB += (int) color(2);
				accumulatorB++;
			}else{
				//cout << "Failed: " << tmp1 << " " << tmp2 << endl;
				//cout << ".";
			}
			x++;
		}
	}

	//cout << "SumR " << sumR << " Acc " << accumulatorR << endl;
	//cout << "SumG " << sumG << " Acc " << accumulatorG << endl;
	//cout << "SumB " << sumB << " Acc " << accumulatorB << endl;
	sumR = (double) (sumR) / (double) accumulatorR;
	sumG = (double) (sumG) / (double) accumulatorG;
	sumB = (double) (sumB) / (double) accumulatorB;
	if(accumulatorR != 0) arr[0] = (int) sumR;
	else arr[0] = img.at<Vec3b>(copya,copyb)(0);
	if(accumulatorG != 0) arr[1] = (int) sumG;
	else arr[1] = img.at<Vec3b>(copya,copyb)(1);
	if(accumulatorB != 0) arr[2] = (int) sumB;
	else arr[2] = img.at<Vec3b>(copya,copyb)(2);

	//cout << "Arrr: " << arr[0] << " " << arr[1] << " " << arr[2] << endl;
	return arr;
}


int *BoxBlur(void*ap){
	Mat blurred, img;
	struct thread_info tinfo;
	int id, sumR = 0, sumG = 0, sumB = 0;
	tinfo = *(thread_info*)ap;
	id = tinfo.id;
	blurred = img = tinfo.img;

	for(int x=id;x<img.cols - 3;x+=THREADS)
	{
		for(int y=0;y<img.rows - 3;y++)
		{
			//	cout << "x: " << x <<"<" <<img.cols  << " y: " << y<<"<" <<img.rows  << endl;
			Vec3b color = img.at<Vec3b>(Point(x,y));
			int *results = getAverageAround(img, x, y, 1);
			color(0) = results[0];
			color(1) = results[1];
			color(2) = results[2];
			img.at<Vec3b>(Point(x,y)) = color;
			//cout << "Previous " << color <<  endl;
			//cout << "results: " << results[0] << " " << results[1] << " "<< results[2] << endl;
			//cout << "After " << color <<  endl;
		}
	}
	return 0;
}

int main(int argc, char** argv ){
		clock_t start, end;
    start=clock();
		gettimeofday(&tv1, NULL);

		if ( argc != 2 ){
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    	}
		Mat img;
    img = imread( argv[1], 1 );
		if ( !img.data ){
        printf("No image data \n");
        return -1;
    }

    int red, green, blue, length, i, r;
		pthread_t hilo[THREADS];
		struct thread_info tinfo;
		tinfo.img = img;

		for(int i = 0; i < ITERATIONS; i++){
			for(i=0; i<THREADS; i++){
				tinfo.id = i;
				r = pthread_create(&hilo[i], NULL, (void*)BoxBlur, (void*)&tinfo);
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

		string name_image =  argv[1];
		vector<std::string> allArgs(argv, argv + argc);
		length = allArgs[1].size();
		cout << "length " << length << endl;
		Mat original_original;
		//char [50] name_new_image;
		//int len = (int) strlen(name_image);
		//int length = sizeof(name_image)/sizeof(char);



// 		std::cout << "Splitting: " << str << '\n';
std::size_t found = allArgs[1].find_last_of('.');
//std::cout << " path: " << allArgs[1].substr(0,found) << '\n';
//std::cout << " file: " << allArgs[1].substr(found+1) << '\n';


  	string name_new_image = allArgs[1].substr(0,found);
		name_new_image += "_blurred.";
		name_new_image += allArgs[1].substr(found+1);
    original_original = imread( argv[1], 1 );
		cout << name_new_image << endl;
		imwrite( name_new_image, img );

		end=clock();
		double time_taken = (double) (end-start)/CLOCKS_PER_SEC;
		gettimeofday(&tv2, NULL);

		cout << "The image " << allArgs[1].substr(0,found) << " with " << img.rows << " rows and " << img.cols << " cols was blurred with " << ITERATIONS << " iterations in " << time_taken << " seconds."<< endl;

		printf ("Total time = %f seconds\n",
	         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec));
		return 0;
}
