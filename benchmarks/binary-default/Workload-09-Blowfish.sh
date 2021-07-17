#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/bf.o e $ALGO_PATH/input_large.asc output_large.enc 1234567890abcdeffedcba0987654321
./$ALGO_PATH/bfv e $ALGO_PATH/input_large.asc output_large.enc 1234567890abcdeffedcba0987654321
./$ALGO_PATH/bf.o e $ALGO_PATH/input_large.asc output_large.enc 1234567890abcdeffedcba0987654321
./$ALGO_PATH/bf.o e $ALGO_PATH/input_large.asc output_large.enc 1234567890abcdeffedcba0987654321
./$ALGO_PATH/bf.o e $ALGO_PATH/input_large.asc output_large.enc 1234567890abcdeffedcba0987654321
