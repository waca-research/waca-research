#!/bin/bash


##############################################################
# Parse all arguments and act as per target
##############################################################

# Extract target from command-line argument
target=$1
shift;

# Extract platform from command-line argument
platform=$1
shift;

# Extract common-dir from command-line argument
common_dir=$1
shift;

# Extract algorithm from command-line argument
algorithm=$1
shift;

# Extract governor from command-line argument
governor=$1
shift;

# Iterate over the workload arguments in the command-line
for arg in $@ ; do

  echo
  if [[ "$arg" == "Workload"* ]] ; then

    echo "#####################################"
    echo "## $arg"
    echo "#####################################"

    if [[ "$target" == "run" ]] ; then
      ${common_dir}/run_characterization_${platform}.sh $arg $platform $algorithm $governor $common_dir
    fi

  else 
    pushd $arg

    if [[ "$target" == "clean" ]] ; then
      make clean
    fi

    if [[ "$target" == "compile" ]] ; then
      make clean && make
    fi

    popd
  fi

done
