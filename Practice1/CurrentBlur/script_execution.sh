#!/bin/bash

rm main
cmake .
make
./main landscape.jpg
