#!/bin/bash

NL=$1
NB=$2
FL=$3
FB=$4
GOV=$5

echo '0' > /sys/devices/odroid_fan.16/fan_mode
echo '91' > /sys/devices/odroid_fan.16/pwm_duty

bash -c "echo $GOV > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
bash -c "echo $GOV > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor"

echo -n "[ INFO] CPU Governor: "

cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
cat /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor

if [ $NL == 1 -a $NB == 1 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 1 -a $NB == 2 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 1 -a $NB == 3 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 1 -a $NB == 4 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 2 -a $NB == 1 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 2 -a $NB == 2 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 2 -a $NB == 3 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 2 -a $NB == 4 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 3 -a $NB == 1 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 3 -a $NB == 2 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 3 -a $NB == 3 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 3 -a $NB == 4 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu7/online"	
elif [ $NL == 4 -a $NB == 1 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 4 -a $NB == 2 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
elif [ $NL == 4 -a $NB == 3 ]; then
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 0 > /sys/devices/system/cpu/cpu7/online"
else
	bash -c "echo 1 > /sys/devices/system/cpu/cpu0/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu3/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu6/online"
	bash -c "echo 1 > /sys/devices/system/cpu/cpu7/online"
fi
	
echo "[ INFO] Online CPUs:"

cat /sys/devices/system/cpu/cpu0/online
cat /sys/devices/system/cpu/cpu1/online
cat /sys/devices/system/cpu/cpu2/online
cat /sys/devices/system/cpu/cpu3/online
cat /sys/devices/system/cpu/cpu4/online
cat /sys/devices/system/cpu/cpu5/online
cat /sys/devices/system/cpu/cpu6/online
cat /sys/devices/system/cpu/cpu7/online

bash -c "echo 50000 > /sys/devices/system/cpu/cpu0/cpufreq/ondemand/sampling_rate"
bash -c "echo 50000 > /sys/devices/system/cpu/cpu4/cpufreq/ondemand/sampling_rate"

echo -n "[ INFO] Sampling Rate: "

cat /sys/devices/system/cpu/cpu0/cpufreq/ondemand/sampling_rate
cat /sys/devices/system/cpu/cpu4/cpufreq/ondemand/sampling_rate

bash -c "echo $FL > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
bash -c "echo $FL > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

bash -c "echo $FB > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq"
bash -c "echo $FB > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq"

echo -n "[ INFO] Frequency of CPUs: "

cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_cur_freq
