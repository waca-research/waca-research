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

    NL=$1
    NB=$2
    FL=$3
    FB=$4

    ##################################################################
    # Set online cores and their frequencies
    ##################################################################
    sleep 1
    ${COMMON_DIR}/core_freq_config_${PLATFORM}.sh $NL $NB $FL $FB $GOVERNOR
    sleep 1
    ${COMMON_DIR}/core_freq_config_${PLATFORM}.sh $NL $NB $FL $FB $GOVERNOR

    TRACE_NAME=$NL"L_"$NB"B_"$FL"L_"$FB"B_DATA"

    ##################################################################
    # Set output path for trace
    ##################################################################
    mkdir -p ${ROOT_DIR}/$TRACE_OUTPUT_PATH/$BENCHMARK_NAME
    mkdir -p ${ROOT_DIR}/output
    COUNTER=0

    ##################################################################
    # Run the benchmark for multiple iterations
    ##################################################################
    while [ $COUNTER -lt 5 ]; do

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

for NL in 4 3 2 1
# for NL in 4
do
    for NB in 4 3 2 1
    # for NB in 4
    do 
        for FL in 1400000 1300000 1200000 1100000 1000000 900000 800000 700000 600000
        # for FL in 1400000 
        do
            if [ "$FL" -eq 1400000 ]; then
                for FB in 2000000 1900000 1800000 1700000 1600000 1500000 1400000
                # for FB in 2000000 
                do
                    collect_data $NL $NB $FL $FB
                done
            else    
                FB=$FL
                collect_data $NL $NB $FL $FB
            fi
        done        
    done
done
