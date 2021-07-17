#!/bin/bash

NC=$1
FC=$2
GOVERNOR=$3

echo $GOVERNOR > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor

echo -n "[ INFO] CPU Governor: "
cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor

if [ $NC == 1  ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 0 > /sys/devices/system/cpu/cpu1/online
	echo 0 > /sys/devices/system/cpu/cpu2/online
	echo 0 > /sys/devices/system/cpu/cpu3/online
	echo 0 > /sys/devices/system/cpu/cpu4/online
	echo 0 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 2 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 0 > /sys/devices/system/cpu/cpu2/online
	echo 0 > /sys/devices/system/cpu/cpu3/online
	echo 0 > /sys/devices/system/cpu/cpu4/online
	echo 0 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 3 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 0 > /sys/devices/system/cpu/cpu3/online
	echo 0 > /sys/devices/system/cpu/cpu4/online
	echo 0 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 4 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo 0 > /sys/devices/system/cpu/cpu4/online
	echo 0 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 5 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo 1 > /sys/devices/system/cpu/cpu4/online
	echo 0 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 6 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo 1 > /sys/devices/system/cpu/cpu4/online
	echo 1 > /sys/devices/system/cpu/cpu5/online
	echo 0 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 7 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo 1 > /sys/devices/system/cpu/cpu4/online
	echo 1 > /sys/devices/system/cpu/cpu5/online
	echo 1 > /sys/devices/system/cpu/cpu6/online
	echo 0 > /sys/devices/system/cpu/cpu7/online
elif [ $NC == 8 ]; then
	echo 1 > /sys/devices/system/cpu/cpu0/online
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo 1 > /sys/devices/system/cpu/cpu4/online
	echo 1 > /sys/devices/system/cpu/cpu5/online
	echo 1 > /sys/devices/system/cpu/cpu6/online
	echo 1 > /sys/devices/system/cpu/cpu7/online
else
	echo "[ERROR] Invalid core count configuration. Provide a CPU count between 1 and 8. Exiting...\n"
        exit 1
fi
	
echo "[ INFO] Online CPUs:"
cat /sys/devices/system/cpu/cpu*/online | sed -e 's/\n/ /g'
# cat /sys/devices/system/cpu/cpu0/online
# cat /sys/devices/system/cpu/cpu1/online
# cat /sys/devices/system/cpu/cpu2/online
# cat /sys/devices/system/cpu/cpu3/online
# cat /sys/devices/system/cpu/cpu4/online
# cat /sys/devices/system/cpu/cpu5/online
# cat /sys/devices/system/cpu/cpu6/online
# cat /sys/devices/system/cpu/cpu7/online

echo 50000 > /sys/devices/system/cpu/cpufreq/ondemand/sampling_rate

echo -n "[ INFO] Sampling Rate: "
cat /sys/devices/system/cpu/cpufreq/ondemand/sampling_rate  

sh -c "echo $FC > /sys/devices/system/cpu/cpufreq/policy0/scaling_min_freq"
sh -c "echo $FC > /sys/devices/system/cpu/cpufreq/policy0/scaling_max_freq"

echo -n "[ INFO] Frequency of CPUs: "
cat /sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq
echo ""
