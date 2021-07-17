#!/bin/bash

BENCHMARK_NAME=$1
PLATFORM=$2
ALGORITHM=$3
GOVERNOR=$4
COMMON_DIR=$5

ROOT_DIR=$(pwd)

TRACE_FILE_NAME=/tmp/power-temp-freq-data.csv
TRACE_OUTPUT_PATH=output

function collect_data()
{
    echo
    echo '[ INFO] Collecting data'
    echo

    NC=$1
    FC=$2

    ##################################################################
    # Set online cores and their frequencies
    ##################################################################
    sleep 1
    ${COMMON_DIR}/core_freq_config_${PLATFORM}.sh $NC $FC $GOVERNOR
    sleep 1
    ${COMMON_DIR}/core_freq_config_${PLATFORM}.sh $NC $FC $GOVERNOR

    TRACE_NAME=$NC"C_"$FC"F_DATA"

    ##################################################################
    # Set output path for trace
    ##################################################################
    mkdir -p ${ROOT_DIR}/$TRACE_OUTPUT_PATH/$BENCHMARK_NAME
    mkdir -p ${ROOT_DIR}/output
    COUNTER=0

    ##################################################################
    # Run the benchmark for multiple iterations
    ##################################################################
    while [ $COUNTER -lt 1 ]; do

	sleep 5

	    echo
	    echo '[ INFO] Workload: ' $BENCHMARK_NAME
	    echo '[ INFO] Iteration: ' $COUNTER
	    echo 

	    pushd $ROOT_DIR
	    $ROOT_DIR/$ALGORITHM/$BENCHMARK_NAME.sh $ALGORITHM
	    popd
		
	mv $TRACE_FILE_NAME $ROOT_DIR/$TRACE_OUTPUT_PATH/$BENCHMARK_NAME/$TRACE_NAME"_"$COUNTER".txt"

	let COUNTER=COUNTER+1;

	sleep 2

    done;
    
}


for NC in 6 5 4 3 2 1
# for NC in 6
do

  # for FC in 1907200 
  for FC in 1907200 1804800 1728000 1651200 1574400 1420800 1344000 1267200 1113600 1036800 960000 806400 729600 652800 576000 
  # for FC in 1907200 1881600 1804800 1728000 1651200 1574400 1497600 1420800 1344000 1267200 1190400 1113600 1036800 960000 883200 806400 729600 652800 576000 499200 422400 345600 268800 192000 115200
 
  do
    collect_data $NC $FC

  done        
done

