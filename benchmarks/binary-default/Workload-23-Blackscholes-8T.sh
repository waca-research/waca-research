#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/blackscholes.o 8 $ALGO_PATH/in_16K.txt output.txt
