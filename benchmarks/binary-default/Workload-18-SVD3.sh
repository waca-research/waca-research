#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/svd3.o $ALGO_PATH/large-svd.txt > output.txt
