#!/bin/sh
ALGO_PATH=$1

./$ALGO_PATH/rijndael.o $ALGO_PATH/input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321
./$ALGO_PATH/rijndael.o $ALGO_PATH/input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321
./$ALGO_PATH/rijndael.o $ALGO_PATH/input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321
./$ALGO_PATH/rijndael.o $ALGO_PATH/input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321
./$ALGO_PATH/rijndael.o $ALGO_PATH/input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321
