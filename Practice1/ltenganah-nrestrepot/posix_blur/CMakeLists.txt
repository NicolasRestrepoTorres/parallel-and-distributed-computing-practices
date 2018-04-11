cmake_minimum_required(VERSION 2.8)
set(THREADS_PREFER_PTHREAD_FLAG ON)
project( blur-effect )
find_package( OpenCV REQUIRED )
find_package(Threads REQUIRED)
add_executable( blur-effect blur-effect.cpp )
target_link_libraries( blur-effect ${OpenCV_LIBS} Threads::Threads)

