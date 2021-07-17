#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/kmeans-large.o $ALGO_PATH/datasets/D31 1024 512 256 > output.txt
