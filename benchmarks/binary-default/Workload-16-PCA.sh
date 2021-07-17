#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/pca-small.o $ALGO_PATH/small.data 1593 256 R > output.txt
