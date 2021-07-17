#!/bin/bash


##############################################################
# Parse all arguments and perform a make for each workload
##############################################################
for arg in $@ ; do

echo
if [[ "$arg" == "Workload"* ]] ; then
  echo "#####################################"
  echo "## $arg"
  echo "#####################################"
else 
  pushd $arg
  make clean
  popd
fi

done
