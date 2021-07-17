#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/vgg16.o 4 ./$ALGO_PATH/filelist.txt ./$ALGO_PATH/results-4T.txt
