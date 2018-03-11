#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void Blur(Mat img) {
	printf("Blublublubluur!\n");
	int red, green, blue, i, j;

	Mat mred(img.rows, img.rows, CV_64FC1,Scalar(255,255,255));

	for(int y=0;y<img.rows-1;y++){
		for(int x=0;x<img.cols-1;x++){
			red = 0;
			green = 0;
			blue = 0;

			for(i=0; i<3; i++){
				for(j=0; j<3; j++){
					Vec3b color = img.at<Vec3b>(Point(x+i,y+j));
					red += (int) color(0);
					green += (int) color(1);
					blue += (int) color(2);
				}
			}
			red=red/9;
			green=green/9;
			blue=blue/9;
			//printf("r:%d, g:%d, b:%d\n", red, green, blue);
			mred.at<Vec3b>(Point(x,y)) = Vec3b(red,green,blue);
		}
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", mred);
    imwrite( "blurred.jpg", mred);
    waitKey(0);
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
    Blur(img);

	printf("r:%d, c:%d\n", img.rows, img.cols);


	return 0;
}
