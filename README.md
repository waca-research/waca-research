WACA - Workload-Conservative Application Characterization on Heterogeneous Mobile Platforms
=

This is the open source release of WACA methodology that includes the instrumented source code, WACA API Library and the scripts to execute benchmark applications on Odroid-XU3, Xavier NX and AGX Xavier.

## Pre-Requisites
The following must be enabled in Odroid-XU3, Xavier NX and AGX Xavier:
   - Performance monitoring unit (PMU)
   - Ondemand power governor
   - N-sample averaging in continuous mode for the power sensors in all 3 platforms (`N=16` for Odroid-XU3, Xavier NX and `N=64` for AGX Xavier)
   - Temperature sensor values through sysfs entries
   - Expose CPU utilizations through sysfs entries for ondemand governor
   - PAPI

## Directory Structure
- `benchmarks/common` directory contains the run scripts for the 3 platforms
- `benchmarks/src` directory contains the files for the WACA API Library
- `benchmarks/mi-bench-default` for MiBench applications
- `benchmarks/cortexsuite-default` for CortexSuite applications
- `benchmarks/parsec-source-default` for PARSEC applications
- `benchmarks/dnn-default` for MiBench applications

## Parameters to cross-check before starting WACA based runs
- Specify the platform (`odroid`, `xavier_nx`, or `agx_xavier`) in `PLATFORM` in `benchmarks/common/common.mk`
- Specify the workloads to execute (`workloads` variable in `benchmarks/Makefile`)
- Cross-check the configurations (number of active cores and operating frequencies) to execute in `benchmarks/common/run_characterization_${PLATFORM}.sh`

## Move to the workspace
- `cd workspace` 

## Clean the workspace
- `make clean_workloads && clean_binaries`

## Compile the applications
- `make compile`

## Execute the applications
- `make run`
- This command will execute all the applications in `${workloads}` in `Makefile` for all configurations specified in `benchmarks/common/run_characterization_${PLATFORM}.sh`
- P.S.: The time to execute depends on the number of configurations and the applications.

## Location of trace files for application characterization
- `benchmarks/output/${PLATFORM}/${WORKLOAD}/*txt` 
   - A text file for each configuration is created and contains the power, execution time, hardware performance counters, CPU utilization, temperature and frequency information.
   - Each text file contains the aforementioned information for each of the snippets in the application in one line
