#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat Blur(Mat img) {
	printf("Blublublubluur!\n");
	int red, green, blue, i, j;

	Mat mred(img.rows-2, img.cols-2, CV_8UC3,Scalar(255,255,255));

	for(int x=0;x<img.rows-1;x++){
		for(int y=0;y<img.cols-1;y++){
			red = 0;
			green = 0;
			blue = 0;
			Vec3b color = img.at<Vec3b>(x,y);

			for(i=0; i<3; i++){
				for(j=0; j<3; j++){
					Vec3b color = img.at<Vec3b>(x,y);
					red += (int) color(0);
					green += (int) color(1);
					blue += (int) color(2);
				}
			}
			red=red/9;
			green=green/9;
			blue=blue/9;
			//printf("r:%d, g:%d, b:%d\n", red, green, blue);
			mred.at<Vec3b>(x,y)[0] = red;
			mred.at<Vec3b>(x,y)[1] = green;
			mred.at<Vec3b>(x,y)[2] = blue;
		}
	}

	return mred;
}

int main(int argc, char** argv ){

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

    //Un Mat funciona como un array en python,
    //cuando uno lo manda a una función, la
    //función altera el Mat original y no a una copia.
		Mat blurred;

		for(int x = 0; x < 100 ; x++){
			blurred = Blur(img);
		}

		printf("img, r:%d, c:%d\n", img.rows, img.cols);
		printf("mred, r:%d, c:%d\n", blurred.rows, blurred.cols);

			imwrite( "blurred.jpg", blurred);


			namedWindow( "Original", CV_WINDOW_AUTOSIZE );
			namedWindow( "Blurred", CV_WINDOW_AUTOSIZE );

			imshow( "Original", img );
			imshow( "Blurred", blurred );
			waitKey(0);




	return 0;
}
