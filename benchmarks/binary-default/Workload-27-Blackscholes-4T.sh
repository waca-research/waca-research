#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/blackscholes_test.o 4 $ALGO_PATH/in_16K.txt output.txt
