NVCC=nvcc

###################################
# These are the default install   #
# locations on most linux distros #
###################################

OPENCV_LIBPATH=/usr/lib
OPENCV_INCLUDEPATH=/usr/include

###################################################
# On Macs the default install locations are below #
###################################################

#OPENCV_LIBPATH=/usr/local/lib
#OPENCV_INCLUDEPATH=/usr/local/include

OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui
CUDA_INCLUDEPATH=/usr/local/cuda-9.1/include

######################################################
# On Macs the default install locations are below    #
# ####################################################

#CUDA_INCLUDEPATH=/usr/local/cuda/include
#CUDA_LIBPATH=/usr/local/cuda/lib

NVCC_OPTS=-O3 -Xcompiler -Wall -Xcompiler -Wextra -m64

GCC_OPTS=-O3 -Wall -Wextra -m64

student: main.o cuda_processing.o  Makefile
	$(NVCC) -o blur-effect main.o cuda_processing.o  -L $(OPENCV_LIBPATH) $(OPENCV_LIBS) $(NVCC_OPTS)

main.o: main.cpp blur-effect.cpp
	g++ -c main.cpp $(GCC_OPTS) -I $(OPENCV_INCLUDEPATH) -I $(CUDA_INCLUDEPATH)

cuda_processing.o: cuda_processing.cu
	nvcc -c cuda_processing.cu $(NVCC_OPTS)

clean:
	rm -f *.o *.png hw
