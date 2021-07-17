#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/spc-large.o $ALGO_PATH/datasets/MINST 1024 512 256 0.707 1 > output.txt
