#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void Blur(Mat img) {
	printf("Blublublubluur!\n");
	int red, green, blue;

	double convMatrix[3][3] = {{0.9,0.9,0.9},
							   {0.9,0.9,0.9},
							   {0.9,0.9,0.9}};

	Mat mred(img.rows-2, img.rows-2, CV_64FC1,Scalar(0,0,0));

	for(int y=0;y<img.rows;y++){
		
		for(int x=0;x<img.cols;x++){
		    // get pixel
		    Vec3b color = img.at<Vec3b>(Point(x,y));
			red = (int) color(0);
			green = (int) color(1);
			blue = (int) color(2);
		    // ... do something to the color ....
			//imageRed[x][y] = red;
			//imageGreen[x][y] = green;
			//imageBlue[x][y] = blue;
		    // set pixel
		    img.at<Vec3b>(Point(x,y)) = Vec3b(0,0,0);
		}
	}
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

	namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", img);
    waitKey(0);
	return 0;
}
