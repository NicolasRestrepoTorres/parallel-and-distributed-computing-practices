#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
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
	printf("r:%d, c:%d\n", img.rows, img.cols);
	for(int y=0;y<img.rows;y++)
	{
		for(int x=0;x<img.cols;x++)
		{
		    // get pixel
		    Vec3b color = img.at<Vec3b>(Point(x,y));
			cout << color << endl;
		    // ... do something to the color ....

		    // set pixel
		    img.at<Vec3b>(Point(x,y)) = color;
		}
	}
	return 0;
}
