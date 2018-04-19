// Homework 1
// Color to Greyscale Conversion

//A common way to represent color images is known as RGBA - the color
//is specified by how much Red, Grean and Blue is in it.
//The 'A' stands for Alpha and is used for transparency, it will be
//ignored in this homework.

//Each channel Red, Blue, Green and Alpha is represented by one byte.
//Since we are using one byte for each color there are 256 different
//possible values for each color.  This means we use 4 bytes per pixel.

//Greyscale images are represented by a single intensity value per pixel
//which is one byte in size.

//To convert an image from color to grayscale one simple method is to
//set the intensity to the average of the RGB channels.  But we will
//use a more sophisticated method that takes into account how the eye
//perceives color and weights the channels unequally.

//The eye responds most strongly to green followed by red and then blue.
//The NTSC (National Television System Committee) recommends the following
//formula for color to greyscale conversion:

//I = .299f * R + .587f * G + .114f * B

//Notice the trailing f's on the numbers which indicate that they are
//single precision floating point constants and not double precision
//constants.

//You should fill in the kernel as well as set the block and grid sizes
//so that the entire image is processed.

#include "utils.h"
#include <stdio.h>



__global__
void rgba_to_greyscale(const uchar4* const rgbaImage,
                       uchar4 * const greyImage,
                       int numRows, int numCols)
{
  //TODO (solved)
  //Fill in the kernel to convert from color to greyscale
  //the mapping from components of a uchar4 to RGBA is:
  // .x -> R ; .y -> G ; .z -> B ; .w -> A
  //
  //The output (greyImage) at each pixel should be the result of
  //applying the formula: output = .299f * R + .587f * G + .114f * B;
  //Note: We will be ignoring the alpha channel for this conversion

  //First create a mapping from the 2D block and grid locations
  //to an absolute 2D location in the image, then use that to
  //calculate a 1D index
  // could also use blockDim.x instead of numCols
  //Columna, fila
  int index = numCols * blockIdx.x + threadIdx.x;
  uchar4 rgbpx = rgbaImage[index];
  // greyImage[index] = (unsigned char)(0.299f * rgbpx.x + 0.587f *
  //   rgbpx.y + 0.114f * rgbpx.z);
    int size_kernel = 3;
    int start_x = blockIdx.x; int start_y = threadIdx.x;
    int accumulator = 0; double red = 0.0; double green = 0.0; double blue = 0.0;
    int current_pixel; bool valid;
    uchar4 rgbpx_current_pixel;
    for(int i = start_x; i < (start_x + size_kernel); i++){
      for(int j = start_y; j < (start_y + size_kernel); j++){
        //
        if (i < 0 || i >= numCols) valid = false;
        else if (j < 0 || j >= numRows) valid = false;
        else valid = true;
        if (valid){
          current_pixel = numCols * i + j;
          rgbpx_current_pixel = rgbaImage[current_pixel];
          red += rgbpx_current_pixel.x;
          green += rgbpx_current_pixel.y;
          blue += rgbpx_current_pixel.z;

          // greyImage[index] = (unsigned char)(0.299f * rgbpx_current_pixel.x +
          //   0.587f * rgbpx_current_pixel.y + 0.114f * rgbpx_current_pixel.z);
            accumulator += 1;
        }
      }
    }


    if(index % 10000 == 0){
    printf("Original %d: %d \n", index, rgbpx.x);
    rgbpx.x = red;
    rgbpx.y = green;
    rgbpx.z = blue;
    printf("Changed %d: %d \n",index, rgbpx.x);

    greyImage[index] =   rgbpx;
    }

    //
    //
  	// int debug = 0;
  	// int id = index;
    //
    //
  	// int accumulator, cornerX, cornerY, sumRGB[] = {0,0,0};
    //
  	// 	//printf("Hilo: %d\n", id);
  	// 	for(int x=id;x<img.cols;x+=THREADS){
  	// 		//printf("hilo: %d, col: %d\n", id, x);
  	// 		for(int y=0;y<img.rows;y++){
  	// 			//sem_wait(&semvar2);
  	// 			//cout << y << endl;
    //
  	// 			sumRGB[0]=0;
  	// 			sumRGB[1]=0;
  	// 			sumRGB[2]=0;
  	// 			accumulator=0;
  	// 			if(k%2==0){
  	// 				cornerX = x-(k-1)/2;
  	// 				cornerY = y-(k-1)/2;
  	// 			}else{
  	// 				cornerX = x-(k-2)/2;
  	// 				cornerY = y-(k-2)/2;
  	// 			}
    //
  	// 			for(int i = 0; i < k; i++){
  	// 				for(int j = 0; j < k; j++){
  	// 					if(checkBounds(cornerX+j,cornerY+i, img.cols, img.rows)){
  	// 						Vec3b color = img.at<Vec3b>(Point(cornerX+j,cornerY+i));
  	// 						sumRGB[0] += (int) color(0);
  	// 						sumRGB[1] += (int) color(1);
  	// 						sumRGB[2] += (int) color(2);
  	// 						accumulator++;
  	// 					}
  	// 				}
  	// 			}
    //
  	// 			if(accumulator == 0){
  	// 				sumRGB[0] = img.at<Vec3b>(Point(x,y))(0);
  	// 				sumRGB[1] = img.at<Vec3b>(Point(x,y))(1);
  	// 				sumRGB[2] = img.at<Vec3b>(Point(x,y))(2);
  	// 			}else{
  	// 				sumRGB[0] = sumRGB[0] / accumulator;
  	// 				sumRGB[1] = sumRGB[1] / accumulator;
  	// 				sumRGB[2] = sumRGB[2] / accumulator;
  	// 			}
  	// 			//sem_post(&semvar2);
  	// 			Vec3b color;
  	// 			color(0) = sumRGB[0];
  	// 			color(1) = sumRGB[1];
  	// 			color(2) = sumRGB[2];
  	// 			#pragma omp critical
  	// 				blurred.at<Vec3b>(Point(x,y)) = color;
    //
    //
  	// 		}
  	// 	}

}




void your_rgba_to_greyscale(const uchar4 * const h_rgbaImage, uchar4 * const d_rgbaImage,
                            uchar4 * const d_greyImage, size_t numRows, size_t numCols)
{
  //You must fill in the correct sizes for the blockSize and gridSize
  //currently only one block with one thread is being launched
  const dim3 gridSize(numRows, 1, 1);  //TODO (solved)
  const dim3 blockSize(numCols, 1, 1);  //TODO (solved)
  rgba_to_greyscale<<<gridSize, blockSize>>>(d_rgbaImage, d_greyImage, numRows, numCols);

  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

}
