// piN.c
// D. Thiebaut
// Computes Pi using N processes under MPI
//
// To compile and run:
// mpicc -o piN piN.cpp
// time mpirun -np 2 ./piN 100000000
//
// Output
// Process 1 of 2 started on beowulf2.  N= 50000000
// Process 0 of 2 started on beowulf2.  N= 50000000
//  50000000 iterations: Pi = 3.14159
//
//  real0m1.251s
//  user0m1.240s
//  sys0m0.000s
//

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#define MANAGER 0
#define ITERATIONS 1
#define MAXTHREADS 32
using namespace cv;
using namespace std;
struct timeval  tv1, tv2;
int THREADS, k;

Mat img;


//--------------------------------------------------------------------
//                         P R O T O T Y P E S
//--------------------------------------------------------------------
void doManager( Mat, int, int );
void doWorker( Mat );

//--------------------------------------------------------------------
//                           M  A  I  N
//--------------------------------------------------------------------
int main(int argc, char *argv[]) {
  int myId, noProcs, nameLen;
  char procName[MPI_MAX_PROCESSOR_NAME];
  int n;

  if ( argc < 2 ) {
    printf( "Syntax: mpirun -np noProcs ./blur-effect img.jpg img_blurred.jpg k\n" );
    return 1;
  }

  // get the number of samples to generate
  // n = atoi( argv[1] );


	int r;

  img = imread( argv[1], 1 );
	if ( !img.data ){
      printf("No image data \n");
      return -1;
  }

	k = atoi(argv[3]);
  n = img.cols;
	blurred = Mat(img.rows, img.cols, CV_8UC3, Scalar(255,255,255));


  //--- start MPI ---
  MPI_Init( &argc, &argv);
  MPI_Comm_rank( MPI_COMM_WORLD, &myId );
  MPI_Comm_size( MPI_COMM_WORLD, &noProcs );
  MPI_Get_processor_name( procName, &nameLen );

  //--- display which process we are, and how many there are ---
  printf( "Process %d of %d started on %s. n = %d\n",
          myId,  noProcs, procName, n );

  //--- farm out the work: 1 manager, several workers ---
  if ( myId == MANAGER )
    doManager( img, n, noProcs );
  else
    doWorker( img );

  //--- close up MPI ---


	imwrite( argv[2], blurred );

	cout << "Image \"" << argv[1] << "\":" << endl;
	printf("Rows: %d Cols: %d\n", img.rows, img.cols);
	printf("Kernel Size: %d \n", k);
	// printf("Threads: %d\n", THREADS);
	// printf("Iterations: %d \n", ITERATIONS);
	//printf("Time: %f\n", end_t - start_t);
	//printf("%f\n", end_t - start_t);
	cout << endl << endl;



	MPI_Finalize();

  return 0;
}

//--------------------------------------------------------------------
// The function to be evaluated
//--------------------------------------------------------------------
bool checkBounds(int a, int b, int bx, int by){
	if(a<0 || a>=bx) return false;
	if(b<0 || b>=by) return false;
	return true;
}

Mat BoxBlur(Mat img, int begin, int end){

  Mat blurred  = img;
	int debug = 0;


	int accumulator, cornerX, cornerY, sumRGB[] = {0,0,0};

		//printf("Hilo: %d\n", id);
		for(int x=begin;x<end;x++){
			//printf("hilo: %d, col: %d\n", id, x);
			for(int y=0;y<img.rows;y++){
				//sem_wait(&semvar2);
				//cout << y << endl;

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
				//sem_post(&semvar2);
				Vec3b color;
				color(0) = sumRGB[0];
				color(1) = sumRGB[1];
				color(2) = sumRGB[2];
				blurred.at<Vec3b>(Point(x,y)) = color;


			}
		}

    return blurred;

}


//--------------------------------------------------------------------
// The manager's main work function.  Note that the function
// can and should be made more efficient (and faster) by sending
// an array of 3 ints rather than 3 separate ints to each worker.
// However the current method is explicit and better highlights the
// communication pattern between Manager and Workers.
//--------------------------------------------------------------------
void doManager( Mat img, int n, int noProcs ) {
	Mat sum = Mat(img.rows, img.cols, CV_8UC3, Scalar(255,255,255));
  double sum0 = 0, sum1;
  double deltaX = 1.0/n;
  int i, begin, end;

  MPI_Status status;

  //--- first send n and bounds of series to all workers ---
  end = n/noProcs;
  for ( i=1; i<noProcs; i++ ) {
    begin = end;
    end   = (i+1) * n / noProcs;

    MPI_Send( &begin, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
    MPI_Send( &end, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
    MPI_Send( &n, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
  }

  //--- perform summation over 1st interval of the series ---
  begin = 0;
  end   = n/noProcs;

  for ( i = begin; i < end; i++ )
    BoxBlur(img, begin, end);

  //--- wait for other half from worker ---
  for ( i=1; i<noProcs; i++ ) {
    MPI_Recv( &sum1, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
    //sum0 += sum1;
  }

  //--- output result ---
  printf( "%d iterations: Pi = %f\n", n, sum0 *deltaX );
}

//--------------------------------------------------------------------
// The worker's main work function.  Same comment as for the
// Manager.  The 3 ints would benefit from being sent in an array.
//--------------------------------------------------------------------
void doWorker( Mat img ) {
  int begin, end, n, i;

  //--- get n and bounds for summation from manager ---
  MPI_Status status;
  MPI_Recv( &begin, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
  MPI_Recv( &end, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
  MPI_Recv( &n, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );

  //--- sum over boundaries received ---
  double sum = 0;
  double deltaX = 1.0/n;

  for ( i=begin; i< end; i++ )
    BoxBlur(img, begin, end);

  //-- send result to manager ---
  MPI_Send( &sum, 1, MPI_DOUBLE, MANAGER, 0, MPI_COMM_WORLD );
}
