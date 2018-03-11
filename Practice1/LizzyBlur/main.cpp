#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat Blur(Mat img) {
	printf("Blublublubluur!\n");
	int red, green, blue, i, j;
	printf("img.rows %d, img.cols %d\n", img.rows, img.cols);
	Mat mred(img.rows-4, img.cols-4, CV_8UC3, Scalar(255,255,255));



	for(int x=0;x<img.rows-3;x++){
		for(int y=0;y<img.cols-3;y++){
			red = 0;
			green = 0;
			blue = 0;
			for(i=0; i<5; i++){
				for(j=0; j<5; j++){
					Vec3b color = img.at<Vec3b>(x,y);
					red += (int) color(0);
					green += (int) color(1);
					blue += (int) color(2);
				}
			}
			//printf("r:%d, g:%d, b:%d\n", red, green, blue);
			mred.at<Vec3b>(x,y) = Vec3b(red/25,green/25,blue/25);
		}
	}

	imwrite( "blurred.jpg", mred);
	printf("mred, r:%d, c:%d\n", mred.rows, mred.cols);
	namedWindow( "Blurred", CV_WINDOW_AUTOSIZE );
	imshow( "Blurred", mred );
	waitKey(0);
}

int main(int argc, char** argv ){

	if ( argc != 2 ){
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

	Mat image;
	image = imread( argv[1], 1 );

	if ( !image.data ){
        printf("No image data \n");
        return -1;
  }
	Blur(image);


	printf("img, r:%d, c:%d\n", image.rows, image.cols);







	return 0;
}
