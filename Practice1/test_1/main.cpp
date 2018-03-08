#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
using namespace cv;
using namespace std;





bool checkBounds(int a, int b, int bx, int by){
	//cout << "hehehe!" << " "<< a << " "<<b << " "<<bx << " "<<by << endl;
	if(a<0 || a>bx) return false;
	if(b<0 || b>by) return false;
	return true;
}

int *getAverageAround(Mat img, int a, int b, int m){


	int tmp1 = 0, tmp2 = 0, sumR = 0,sumG = 0,sumB = 0, copya = a, copyb = b, x = 1;
	static int arr[3];
	int accumulatorR = 0, accumulatorG = 0, accumulatorB = 0;
	for(int i = 0; i < m*9; i++){
		a = copya; b = copyb;
		//a *= x; b*= x;
		//cout << a << " " << b << " " << x << " " << endl ;
		switch(i%9){

			case 0:
				tmp1 = a-1;
				tmp2 = b-1;
				break;
			case 1:
				tmp1 = a;
				tmp2 = b-1;
				break;
			case 2:
				tmp1 = a+1;
				tmp2 = b-1;
				break;
			case 3:
				tmp1 = a-1;
				tmp2 = b;
				break;
			case 4:
				tmp1 = a;
				tmp2 = b;
				break;
			case 5:
				tmp1 = a+1;
				tmp2 = b;
				break;
			case 6:
				tmp1 = a-1;
				tmp2 = b+1;
				break;
			case 7:
				tmp1 = a;
				tmp2 = b+1;
				break;
			case 8:
				tmp1 = a+1;
				tmp2 = b+1;

				break;
		}
		//cout <<  "a / b: " << a << " " << b << " case: " << i << " " << i%9 << " Tmps: " << tmp1 << " " << tmp2 << " Bounds damaged? " << checkBounds(tmp1,tmp2, img.cols, img.rows)<< endl;

		if(checkBounds(tmp1,tmp2, img.cols, img.rows)){
			//cout << Point(tmp1,tmp2) << endl;
			Vec3b color = img.at<Vec3b>(Point(tmp1,tmp2));
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







Mat BoxBlur(Mat img){
	cout << "check";
		//cout << "rows/cols " << img.rows << " " << img.cols << endl;;
	Mat blurred = img;

int sumR = 0, sumG = 0, sumB = 0;
	for(int x=0;x<img.cols - 3;x++)
	{
		for(int y=0;y<img.rows - 3;y++)
		{
			//if (x == 1330) continue;
			//if(y==67 || y==268 || y==402 || y == 1330 || y==428) continue;
			//if (x == 428) continue;
				// get pixel

						//	cout << "x: " << x <<"<" <<img.cols  << " y: " << y<<"<" <<img.rows  << endl;
							 Vec3b color = img.at<Vec3b>(Point(x,y));
							int *results = getAverageAround(img, x, y, 1);
							color(0) = results[0];
							color(1) = results[1];
							color(2) = results[2];
							 img.at<Vec3b>(Point(x,y)) = color;






				//cout << "Previous " << (int) color(0) << " Av " << getAverageAround(img, x, y, 1204) << endl;




				//cout << "Previous " << color <<  endl;
				//cout << "Previous " << img.at<Vec3b>(Point(x,y)) <<  endl;
				//cout << "results: " << results[0] << " " << results[1] << " "<< results[2] << endl;

				//img.setcolor(cv::Vec3b(getAverageAround(img, x, y, 1204),(int) color(1),(int) color(2)));


				//if (x>100) break;
				//if (x>10) break;
				//img.at<Vec3b>(Point(x,y))(0) = getAverageAround(img, x, y, 1204);
				//cout << "After " << img.at<Vec3b>(Point(x,y)) <<  endl;
				//cout << "After " << color <<  endl;
				//if(x>11)break;
			//	cout << "Get Avergage " << getAverageAround(img, x, y, 1204) << endl;

			//red = (int) color(0);
			//green = (int) color(1);
			//blue = (int) color(2);
			//sumR += red;
				// ... do something to the color ....
			//imageRed[x][y] = red;
			//imageGreen[x][y] = green;
			//imageBlue[x][y] = blue;
				// set pixel
				//img.at<Vec3b>(Point(x,y)) = color;
		}

	//break;
	//if (x == 428) continue;
	}

	return img;

}




int *fun()
{
   static int arr[100];

   /* Some operations on arr[] */
   arr[0] = 10;
   arr[1] = 20;

   return arr;
}




int *multiplyRGB(Vec3b component1, Vec3b component2){

	int R1, G1, B1, R2, G2, B2;
	static int arr[3];
	R1 = (int) component1(0);
	G1 = (int) component1(1);
	B1 = (int) component1(2);
	R2 = (int) component2(0);
	G2 = (int) component2(1);
	B2 = (int) component2(2);
	cout << G2 << "Lizzy <3" << endl;
	arr[0] = R1*R2;
	cout << "arr[0] " << arr[0] << " " << R1 << " " << R2;
	arr[1] = G1*G2;
	arr[2] = B1*B2;

	return arr;

}


int *addRGB(Vec3b component1, Vec3b component2){

	int R1, G1, B1, R2, G2, B2;
	static int arr[3];
	R1 = (int) component1(0);
	G1 = (int) component1(1);
	B1 = (int) component1(2);
	R2 = (int) component2(0);
	G2 = (int) component2(1);
	B2 = (int) component2(2);
	cout << G2 << "Lizzy <3" << endl;
	arr[0] = R1+R2;
	cout << "arr[0] " << arr[0] << " " << R1 << " " << R2;
	arr[1] = G1+G2;
	arr[2] = B1+B2;

	return arr;

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
    // int sizeX = (int) img.rows - 2;
    // int sizeY = (int) img.cols - 2;
    // printf("%d\n", sizeY);
    //Mat mred(sizeX-2, sizeY-2, CV_64FC1,Scalar(0.9,0.9,0.9));

		//A = Mat::Mat(sizeX, sizeY, CV_64FC1, void* data, size_t step=AUTO_STEP)

    int red, green, blue;
	// printf("r:%d, c:%d\n", img.rows, img.cols);
	//
	// printf("Hier ist da: %d %d\n", mred.rows, mred.cols);

// img.at<Vec3b>(Point(20,52)) *= mred.at<Vec3b>(Point(20,52));

	// for(int y=0;y<img.rows;y++)
	// {
	// 	for(int x=0;x<img.cols;x++)
	// 	{
	// 	    // get pixel
	// 	    Vec3b color = img.at<Vec3b>(Point(x,y));
	// 		red = (int) color(0);
	// 		green = (int) color(1);
	// 		blue = (int) color(2);
	//
	// 	    // ... do something to the color ....
	// 		//imageRed[x][y] = red;
	// 		//imageGreen[x][y] = green;
	// 		//imageBlue[x][y] = blue;
	// 	    // set pixel
	// 	    img.at<Vec3b>(Point(x,y)) = color;
	// 	}
	// }

// Vec3b any = img.at<Vec3b>(Point(20,52));
// Vec3b anyii = mred.at<Vec3b>(Point(20,52));
// int *arrx = multiplyRGB(any, anyii);
//
//
// cout << arrx[0] << "jeje" << endl;

cout << "check";
	Mat changed;



	for(int lizzy = 0; lizzy < 15; lizzy++)
	changed = BoxBlur(img);


cout << "rows/cols " << img.rows << " " << img.cols;
	//for(int h= 0; h<= 45; h++)	changed = BoxBlur(changed);
	//int arrx[4];


//	int *ptr = fun();
	//cout << ptr[0] << " " << ptr[1];
	//return 0



	Mat original_original;
	string name_image = argv[1];

  //std::replace( name_image.begin(), name_image.end(), 'jpg', ' '); // replace all 'x' to 'y'
	//  std::replace( name_image.begin(), name_image.end(), '.', ' '); // replace all 'x' to 'y'
	name_image += "_blurred.jpg";
    original_original = imread( argv[1], 1 );
	imwrite( name_image, changed );
 //
//namedWindow( "Original", CV_WINDOW_AUTOSIZE );
 //namedWindow( "Blurred", CV_WINDOW_AUTOSIZE );

 //imshow( "Original", original_original );
 //imshow( "Blurred", changed );
cout << endl;
 //waitKey(0);
	return 0;
}
