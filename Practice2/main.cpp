#include <iostream>
#include "timer.h"
#include <string>
#include <stdio.h>
#include "blur-effect.cpp"

void box_blur(const uchar4 * const h_inputImageRGBA, uchar4 * const d_inputImageRGBA,
                        uchar4* const d_outputImageRGBA,
                        const size_t numRows, const size_t numCols,
                        unsigned char *d_redBlurred,
                        unsigned char *d_greenBlurred,
                        unsigned char *d_blueBlurred,
                        const int filterWidth);

void allocateMemoryAndCopyToGPU(const size_t numRowsImage, const size_t numColsImage,
                                const float* const h_filter, const size_t filterWidth);



int main(int argc, char **argv) {
  uchar4 *h_inputImageRGBA,  *d_inputImageRGBA;
  uchar4 *h_outputImageRGBA, *d_outputImageRGBA;
  unsigned char *d_redBlurred, *d_greenBlurred, *d_blueBlurred;

  float *h_filter;
  int    filterWidth;

  std::string input_file;
  std::string output_file;
  std::string reference_file;
  double perPixelError = 0.0;
  double globalError   = 0.0;
  bool useEpsCheck = false;
  switch (argc)
  {
	case 2:
	  input_file = std::string(argv[1]);
	  output_file = "blur-effect_output.png";
	  reference_file = "blur-effect_reference.png";
	  break;
	case 3:
	  input_file  = std::string(argv[1]);
      output_file = std::string(argv[2]);
	  reference_file = "blur-effect_reference.png";
	  break;
	case 4:
	  input_file  = std::string(argv[1]);
      output_file = std::string(argv[2]);
	  reference_file = std::string(argv[3]);
	  break;
	case 6:
	  useEpsCheck=true;
	  input_file  = std::string(argv[1]);
	  output_file = std::string(argv[2]);
	  reference_file = std::string(argv[3]);
	  perPixelError = atof(argv[4]);
      globalError   = atof(argv[5]);
	  break;
	default:
  std::cout << argv;
      std::cerr << "Usage: ./blur-effect input_file [output_filename] [reference_filename] [perPixelError] [globalError]" << std::endl;
      exit(1);
  }
  preProcess(&h_inputImageRGBA, &h_outputImageRGBA, &d_inputImageRGBA, &d_outputImageRGBA,
             &d_redBlurred, &d_greenBlurred, &d_blueBlurred,
             &h_filter, &filterWidth, input_file);

  allocateMemoryAndCopyToGPU(numRows(), numCols(), h_filter, filterWidth);
  GpuTimer timer;
  timer.Start();
  box_blur(h_inputImageRGBA, d_inputImageRGBA, d_outputImageRGBA, numRows(), numCols(),
                     d_redBlurred, d_greenBlurred, d_blueBlurred, filterWidth);
  timer.Stop();
  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
  std::cout << "The image " << std::string(argv[1]);
  int err = printf(" was blurred in: %f miliseconds.\n", timer.Elapsed());

  if (err < 0) {
    std::cerr << "There was errors in blurring" << std::endl;
    exit(1);
  }


  size_t numPixels = numRows()*numCols();
  checkCudaErrors(cudaMemcpy(h_outputImageRGBA, d_outputImageRGBA__, sizeof(uchar4) * numPixels, cudaMemcpyDeviceToHost));

  postProcess(output_file, h_outputImageRGBA);

  checkCudaErrors(cudaFree(d_redBlurred));
  checkCudaErrors(cudaFree(d_greenBlurred));
  checkCudaErrors(cudaFree(d_blueBlurred));

  cleanUp();

  return 0;
}
