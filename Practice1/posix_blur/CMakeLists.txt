cmake_minimum_required(VERSION 2.8)
set(THREADS_PREFER_PTHREAD_FLAG ON)
project( main_posix )
find_package( OpenCV REQUIRED )
find_package(Threads REQUIRED)
add_executable( main_posix main_posix.cpp )
target_link_libraries( main_posix ${OpenCV_LIBS} Threads::Threads)

