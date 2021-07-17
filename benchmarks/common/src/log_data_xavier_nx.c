#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <math.h>

#include "log_data.h"

#define FILE_SIZE 512

//#define DYPO 

#define NORM_PARA 10000

#define NUM_CLASS 6
#define NUM_FEATURES 11

//--------------------------------------------------------------------Dypo--------------------------------------------------------------------

// Beta values
float Beta[NUM_CLASS][NUM_FEATURES] = {
/* 	{-0.828252428,	-0.030401666,	2.00E-03,		2.02E-05,		0.004421668,	-0.015739247,	0.00557657,		0.016428278,	0.433701363,	0.054694028,	0.044515595},
	{-4.600247883,	-0.038399874,	-0.001440483,	-0.000135965,	-1.93E-02,		0.001448927,	0.008046064,	0.024127146,	0.425193122,	0.066068809,	0.050870649},
	{-7.131786163,	-0.029391508,	-0.000806906,	-0.000758547,	-0.022790242,	-0.00177794,	0.010103897,	0.02112495,		0.544607572,	0.045477607,	0.048353898},
	{-4.167895331,	-0.001144229,	-3.05E-05,		-1.85E-03,		-0.066396692,	0.001541204,	-7.75E-05,		0.07978087,		0.673809752,	0.063147668,	0.027939934},
	{-2.721743769,	-0.023877813,	0.003471161,	-9.63E-06,		-0.013822367,	-0.013907406,	0.001409289,	0.010202193,	0.398261881,	0.052434503,	0.036922764},
	{-1.220985631,	-0.122035704,	-0.012560502,	0.000387896,	0.228548476,	0.055762065,	0.003702097,	0.069740919,	0.136569412,	0.052441067,	0.041080946},
	{-8.680754939,	-2.45E-02,		0.002936971,	4.67E-05,		0.009096065,	-0.015307232,	0.00118268,		-0.000175992,	0.437670851,	0.071677405,	0.062757611},
	{-12.15292521,	-0.022220764,	-0.002504547,	-4.09E-03,		-0.030071867,	5.34E-05,		0.009124574,	0.126768784,	0.42707587,		0.083495578,	0.05008407},
	{-0.233278288,	0.006530672,	3.82E-03,		-3.44E-03,		0.002395399,	-0.024679393,	-0.004459758,	0.08772041,		-0.56760456,	0.014483534,	-0.001712662},
	{-0.899291357,	0.033446132,	-1.88E-03,		1.63E-04,		-0.002151883,	0.040114056,	-1.15E-02,		0.001122616,	0.690158205,	-0.004738323,	0.005279685},
	{-0.344302041,	-1.33E-06,		1.32E-05,		-4.12E-06,		1.29E-05,		-5.77E-06,		-7.66E-06,		8.45E-05,		0.038299939,	0.00136778,		-0.00047857},
	{-2.329719854,	1.35E-02,		1.58E-03,		-1.53E-03,		-2.22E-02,		-3.93E-02,		-1.18E-06,		0.077857826,	0.650198452,	0.002169548,	0.003509702},
	{-2.601787199,	0.003215399,	1.43E-03,		-2.20E-03,		0.00717726,		-0.020749396,	-3.47E-05,		0.086417076,	0.11494731,		0.017026651,	0.004482483},
	{0.013909099,	-1.06E-04,		1.76E-04,		1.40E-05,		-1.32E-03,		-5.49E-04,		3.24E-05,		-1.08E-04,		-0.011461311,	-6.31E-04,		1.42E-03} */

	{3.9896,	-0.0059,	0.0218,	-0.0005,	-0.2598,	-0.0644,	-0.0215, -0.0434,	0.3042,	0.0028,	0.0034},
	{1.9773,	-0.0072,	0.0225,	-0.0007,	-0.206,		-0.0696,	-0.0202, -0.064,	0.3406,	-0.0099,	0.0156},
	{-5.1165,	-0.0096,	0.014,	-0.0021,	-0.1984,	-0.1351,	0.0445,	 -0.1216,	1.3005,	0.0146,	0.0365},
	{3.2894,	0.0247, 	-0.0211,	-0.0002,	0.3191,	0.0634,		0.0106,	 0.1041,	-0.0467,	0.021,	-0.027},
	
};

/* float BETA_1[] = {
5.6976,
0.0001,
0.0145,
-0.0032,
0.129,
-0.0329,
-0.0239,
-0.0232,
0.2263
};

float BETA_2[] = {
2.3584,
-0.0052,
0.0212,
-0.0052,
0.0485,
-0.0539,
-0.0232,
-0.0491,
0.2556
};

float BETA_3[] = {
3.2116,
0.0053,
0.0168,
-0.0014,
0.0911,
-0.0517,
-0.0228,
-0.05,
0.1507
};                 

float BETA_4[] = {
2.5332,
-0.0038,
0.0234,
-0.0058,
0.3113,
-0.0406,
-0.0469,
-0.0322,
0.2427
};  */  

//--------------------------------------------------------------------Dypo--------------------------------------------------------------------

static FILE *fp;
char filename[FILE_SIZE];

// // A7 Power
// char A7_TEMP[FILE_SIZE];
// int A7_FILE, A7_READ;
// //float A7_POWER=0.0;
// char A7_POWER[FILE_SIZE];

// CPU_GPU Power
char CPU_GPU_TEMP[FILE_SIZE];
int CPU_GPU_FILE, CPU_GPU_READ;
//float CPU_GPU_POWER=0.0;
char CPU_GPU_POWER[FILE_SIZE];

// SYSTEM Power
char SYSTEM_TEMP[FILE_SIZE];
int SYSTEM_FILE, SYSTEM_READ;
//float SYSTEM_POWER=0.0;
char SYSTEM_POWER[FILE_SIZE];

// SOC Power
char SOC_TEMP[FILE_SIZE];
int SOC_FILE, SOC_READ;
//float SOC_POWER=0.0;
char SOC_POWER[FILE_SIZE];

// // A15 Power
// char A15_TEMP[FILE_SIZE];
// int A15_FILE, A15_READ;
// //float A15_POWER=0.0;
// char A15_POWER[FILE_SIZE];

// // Memory Power
// char MEM_TEMP[FILE_SIZE];
// int MEM_FILE, MEM_READ;
// //float MEM_POWER=0.0;
// char MEM_POWER[FILE_SIZE];

// // GPU Power
// char GPU_TEMP[FILE_SIZE];
// int GPU_FILE, GPU_READ;
// //float GPU_POWER=0.0;
// char GPU_POWER[FILE_SIZE];

// Temperature CPU

char TEMP_CPU0_TEMP[FILE_SIZE];
int TEMP_CPU0_FILE, TEMP_CPU0_READ;

char TEMP_CPU1_TEMP[FILE_SIZE];
int TEMP_CPU1_FILE, TEMP_CPU1_READ;

char TEMP_CPU2_TEMP[FILE_SIZE];
int TEMP_CPU2_FILE, TEMP_CPU2_READ;

char TEMP_CPU3_TEMP[FILE_SIZE];
int TEMP_CPU3_FILE, TEMP_CPU3_READ;

char TEMP_CPU4_TEMP[FILE_SIZE];
int TEMP_CPU4_FILE, TEMP_CPU4_READ;

char TEMP_CPU5_TEMP[FILE_SIZE];
int TEMP_CPU5_FILE, TEMP_CPU5_READ;



// Utilization

char CPU0_UTIL[FILE_SIZE];
int CPU0_UTIL_FILE, CPU0_UTIL_READ;
//int CPU0_UTIL_VALUE=0;
char CPU0_UTIL_VALUE[FILE_SIZE];

char CPU1_UTIL[FILE_SIZE];
int CPU1_UTIL_FILE, CPU1_UTIL_READ;
//int CPU1_UTIL_VALUE=0;
char CPU1_UTIL_VALUE[FILE_SIZE];

char CPU2_UTIL[FILE_SIZE];
int CPU2_UTIL_FILE, CPU2_UTIL_READ;
//int CPU2_UTIL_VALUE=0;
char CPU2_UTIL_VALUE[FILE_SIZE];

char CPU3_UTIL[FILE_SIZE];
int CPU3_UTIL_FILE, CPU3_UTIL_READ;
//int CPU3_UTIL_VALUE=0;
char CPU3_UTIL_VALUE[FILE_SIZE];

char CPU4_UTIL[FILE_SIZE];
int CPU4_UTIL_FILE, CPU4_UTIL_READ;
//int CPU4_UTIL_VALUE=0;
char CPU4_UTIL_VALUE[FILE_SIZE];

char CPU5_UTIL[FILE_SIZE];
int CPU5_UTIL_FILE, CPU5_UTIL_READ;
//int CPU5_UTIL_VALUE=0;
char CPU5_UTIL_VALUE[FILE_SIZE];

// char CPU6_UTIL[FILE_SIZE];
// int CPU6_UTIL_FILE, CPU6_UTIL_READ;
// //int CPU6_UTIL_VALUE=0;
// char CPU6_UTIL_VALUE[FILE_SIZE];

// char CPU7_UTIL[FILE_SIZE];
// int CPU7_UTIL_FILE, CPU7_UTIL_READ;
// //int CPU7_UTIL_VALUE=0;
// char CPU7_UTIL_VALUE[FILE_SIZE];

// CPU Online/Offline

char CPU0_OF[FILE_SIZE];
int CPU0_OF_FILE, CPU0_OF_READ;
//int CPU0_OF_VALUE=0;
char CPU0_OF_VALUE[FILE_SIZE];

char CPU1_OF[FILE_SIZE];
int CPU1_OF_FILE, CPU1_OF_READ;
//int CPU1_OF_VALUE=0;
char CPU1_OF_VALUE[FILE_SIZE];

char CPU2_OF[FILE_SIZE];
int CPU2_OF_FILE, CPU2_OF_READ;
//int CPU2_OF_VALUE=0;
char CPU2_OF_VALUE[FILE_SIZE];

char CPU3_OF[FILE_SIZE];
int CPU3_OF_FILE, CPU3_OF_READ;
//int CPU3_OF_VALUE=0;
char CPU3_OF_VALUE[FILE_SIZE];

char CPU4_OF[FILE_SIZE];
int CPU4_OF_FILE, CPU4_OF_READ;
//int CPU4_OF_VALUE=0;
char CPU4_OF_VALUE[FILE_SIZE];

char CPU5_OF[FILE_SIZE];
int CPU5_OF_FILE, CPU5_OF_READ;
//int CPU5_OF_VALUE=0;
char CPU5_OF_VALUE[FILE_SIZE];

// char CPU6_OF[FILE_SIZE];
// int CPU6_OF_FILE, CPU6_OF_READ;
// //int CPU6_OF_VALUE=0;
// char CPU6_OF_VALUE[FILE_SIZE];

// char CPU7_OF[FILE_SIZE];
// int CPU7_OF_FILE, CPU7_OF_READ;
// //int CPU7_OF_VALUE=0;
// char CPU7_OF_VALUE[FILE_SIZE];

// Performance counters
/* char PC1[FILE_SIZE];
int PC1_FILE, PC1_READ;
char PC1_VALUE[FILE_SIZE];

char PC2[FILE_SIZE];
int PC2_FILE, PC2_READ;
char PC2_VALUE[FILE_SIZE];

char PC3[FILE_SIZE];
int PC3_FILE, PC3_READ;
char PC3_VALUE[FILE_SIZE];

char PC4[FILE_SIZE];
int PC4_FILE, PC4_READ;
char PC4_VALUE[FILE_SIZE];

char PC5[FILE_SIZE];
int PC5_FILE, PC5_READ;
char PC5_VALUE[FILE_SIZE];

char PC6[FILE_SIZE];
int PC6_FILE, PC6_READ;
char PC6_VALUE[FILE_SIZE]; */

// Write performance counter values to sysfs
void write_data(long long *values,long long microseconds){
	
	// creating a file pointer
	FILE *fp; 
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_1","w"); // file pointer to perf_count_1 in ondemand governor
	fprintf(fp,"%lld",values[0]);  // write instruction retired value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_2","w"); // file pointer to perf_count_2 in ondemand governor
	fprintf(fp,"%lld",values[1]);  // write CPU Cycles value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_3","w"); // file pointer to perf_count_3 in ondemand governor
	fprintf(fp,"%lld",values[2]);  // write Branch Miss Prediction value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_4","w"); // file pointer to perf_count_4 in ondemand governor
	fprintf(fp,"%lld",values[3]);  // write Level 2 cache misses value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_5","w"); // file pointer to perf_count_5 in ondemand governor
	fprintf(fp,"%lld",values[4]);  // write Data Memory Access value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/perf_count_6","w"); // file pointer to perf_count_6 in ondemand governor
	fprintf(fp,"%lld",values[5]);  // write NONCACHE_EXTERNAL_MEMORY_REQUEST value
	fclose(fp); // close the pointer
	
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/ondemand/exec_time","w"); // file pointer to exec_time in ondemand governor
	fprintf(fp,"%lld",microseconds);  // write exec_time value
	fclose(fp); // close the pointer

	
}

// Sorting logic
float *array;

int cmp(const void *a, const void *b){
    int ia = *(int *)a;
    int ib = *(int *)b;
    return array[ia] < array[ib] ? -1 : array[ia] > array[ib];
}


// Change online-offline cores
int change_online_offline_cores(int core, int status){

	// Create a file pointer
	FILE *fp_core;
	
	switch(core){
			case 0:
				fp_core = fopen("/sys/devices/system/cpu/cpu0/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);
			case 1:
				fp_core = fopen("/sys/devices/system/cpu/cpu1/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);
			case 2:
				fp_core = fopen("/sys/devices/system/cpu/cpu2/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);			
			case 3:
				fp_core = fopen("/sys/devices/system/cpu/cpu3/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);
			case 4:
				fp_core = fopen("/sys/devices/system/cpu/cpu4/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);
			case 5:
				fp_core = fopen("/sys/devices/system/cpu/cpu5/online","w");
				fprintf(fp_core,"%d",status);
				fclose(fp_core);
			// case 6:
			// 	fp_core = fopen("/sys/devices/system/cpu/cpu6/online","w");
			// 	fprintf(fp_core,"%d",status);
			// 	fclose(fp_core);
			// case 7:
			// 	fp_core = fopen("/sys/devices/system/cpu/cpu7/online","w");
			// 	fprintf(fp_core,"%d",status);
			// 	fclose(fp_core);
		}
	
	return 0;

}

// Change min A15 Frequency
int change_min_A15_freq(int ref_freq){

	// Create a file pointer
	FILE *fp_freq;

	// Scale min frequency
	fp_freq = fopen("/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq","w");
	fprintf(fp_freq,"%d",ref_freq);
	fclose(fp_freq);
	return 0;

}

// Change max A15 Frequency
int change_max_A15_freq(int ref_freq){

	// Create a file pointer
	FILE *fp_freq;

	// Scale min frequency
	fp_freq = fopen("/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq","w");
	fprintf(fp_freq,"%d",ref_freq);
	fclose(fp_freq);
	return 0;

}

// Change min A7 Frequency
int change_min_A7_freq(int ref_freq){

	// Create a file pointer
	FILE *fp_freq;

	// Scale min frequency
	fp_freq = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq","w");
	fprintf(fp_freq,"%d",ref_freq);
	fclose(fp_freq);
	return 0;

}

// Change max A7 Frequency
int change_max_A7_freq(int ref_freq){

	// Create a file pointer
	FILE *fp_freq;

	// Scale min frequency
	fp_freq = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq","w");
	fprintf(fp_freq,"%d",ref_freq);
	fclose(fp_freq);
	return 0;

}


// Log the performance, power, frequency, temperature and core online/offline
void log_data(long long *values, int tid, long long microseconds, long long timestamp){
	
		// Frequencies CPU

		char CPU0_TEMP[FILE_SIZE];
		int CPU0_FILE, CPU0_READ;
		//float CPU0_FREQ=0.0;
		char CPU0_FREQ[FILE_SIZE]={0}; 

		char CPU1_TEMP[FILE_SIZE];
		int CPU1_FILE, CPU1_READ;
		//float CPU1_FREQ=0.0;
		char CPU1_FREQ[FILE_SIZE]={0}; 

		char CPU2_TEMP[FILE_SIZE];
		int CPU2_FILE, CPU2_READ;
		//float CPU2_FREQ=0.0;
		char CPU2_FREQ[FILE_SIZE]={0};

		char CPU3_TEMP[FILE_SIZE];
		int CPU3_FILE, CPU3_READ;
		//float CPU3_FREQ=0.0;
		char CPU3_FREQ[FILE_SIZE]={0};

		char CPU4_TEMP[FILE_SIZE];
		int CPU4_FILE, CPU4_READ;
		//float CPU4_FREQ=0.0;
		char CPU4_FREQ[FILE_SIZE]={0};

		char CPU5_TEMP[FILE_SIZE];
		int CPU5_FILE, CPU5_READ;
		//float CPU5_FREQ=0.0;
		char CPU5_FREQ[FILE_SIZE]={0};

		sprintf(filename,"/tmp/power-temp-freq-data.csv");

		fp  = fopen(filename,"a+");
		fprintf(fp,"\n");

		// Power 

		// Channel-0 is the total board/rail (system) power
		SYSTEM_FILE = open("/sys/bus/i2c/drivers/ina3221x/7-0040/iio:device0/in_power0_input", O_RDONLY);
		SYSTEM_READ = read(SYSTEM_FILE, SYSTEM_POWER, sizeof(SYSTEM_POWER));
		SYSTEM_POWER[strlen(SYSTEM_POWER)-1] = '\0'; 
		close(SYSTEM_FILE);

		// Channel-1 is the power of CPU+GPU
		CPU_GPU_FILE = open("/sys/bus/i2c/drivers/ina3221x/7-0040/iio:device0/in_power1_input", O_RDONLY);
		CPU_GPU_READ = read(CPU_GPU_FILE, CPU_GPU_POWER, sizeof(CPU_GPU_POWER));
		CPU_GPU_POWER[strlen(CPU_GPU_POWER)-1] = '\0'; 
		close(CPU_GPU_FILE);

		// Channel-2 is the power of the rest of the SoC
		SOC_FILE = open("/sys/bus/i2c/drivers/ina3221x/7-0040/iio:device0/in_power2_input", O_RDONLY);
		SOC_READ = read(SOC_FILE, SOC_POWER, sizeof(SOC_POWER));
		SOC_POWER[strlen(SOC_POWER)-1] = '\0'; 
		close(SOC_FILE);

		// Temperature
		// 6 thermal zones
		TEMP_CPU0_FILE = open("/sys/devices/virtual/thermal/thermal_zone0/temp", O_RDONLY);
		TEMP_CPU0_READ = read(TEMP_CPU0_FILE, TEMP_CPU0_TEMP, sizeof(TEMP_CPU0_TEMP));
		TEMP_CPU0_TEMP[strlen(TEMP_CPU0_TEMP)-1] = '\0'; 
		close(TEMP_CPU0_FILE);

		TEMP_CPU1_FILE = open("/sys/devices/virtual/thermal/thermal_zone1/temp", O_RDONLY);
		TEMP_CPU1_READ = read(TEMP_CPU1_FILE, TEMP_CPU1_TEMP, sizeof(TEMP_CPU1_TEMP));
		TEMP_CPU1_TEMP[strlen(TEMP_CPU1_TEMP)-1] = '\0'; 
		close(TEMP_CPU1_FILE);

		TEMP_CPU2_FILE = open("/sys/devices/virtual/thermal/thermal_zone2/temp", O_RDONLY);
		TEMP_CPU2_READ = read(TEMP_CPU2_FILE, TEMP_CPU2_TEMP, sizeof(TEMP_CPU2_TEMP));
		TEMP_CPU2_TEMP[strlen(TEMP_CPU2_TEMP)-1] = '\0'; 
		close(TEMP_CPU2_FILE);

		TEMP_CPU3_FILE = open("/sys/devices/virtual/thermal/thermal_zone3/temp", O_RDONLY);
		TEMP_CPU3_READ = read(TEMP_CPU3_FILE, TEMP_CPU3_TEMP, sizeof(TEMP_CPU3_TEMP));
		TEMP_CPU3_TEMP[strlen(TEMP_CPU3_TEMP)-1] = '\0'; 
		close(TEMP_CPU3_FILE);

		TEMP_CPU4_FILE = open("/sys/devices/virtual/thermal/thermal_zone4/temp", O_RDONLY);
		TEMP_CPU4_READ = read(TEMP_CPU4_FILE, TEMP_CPU4_TEMP, sizeof(TEMP_CPU4_TEMP));
		TEMP_CPU4_TEMP[strlen(TEMP_CPU4_TEMP)-1] = '\0'; 
		close(TEMP_CPU4_FILE);

		TEMP_CPU5_FILE = open("/sys/devices/virtual/thermal/thermal_zone5/temp", O_RDONLY);
		TEMP_CPU5_READ = read(TEMP_CPU5_FILE, TEMP_CPU5_TEMP, sizeof(TEMP_CPU5_TEMP));
		TEMP_CPU5_TEMP[strlen(TEMP_CPU5_TEMP)-1] = '\0'; 
		close(TEMP_CPU5_FILE);

		// Frequency
		CPU0_FILE = open("/sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq", O_RDONLY);
		CPU0_READ = read(CPU0_FILE, CPU0_FREQ, sizeof(CPU0_FREQ));
		CPU0_FREQ[strlen(CPU0_FREQ)-1] = '\0';
		/* CPU0_READ = read(CPU0_FILE, CPU0_TEMP, sizeof(CPU0_TEMP));
		CPU0_FREQ = strtod(CPU0_TEMP,NULL);
		CPU0_FREQ = CPU0_FREQ/1000000; */
		close(CPU0_FILE);

		// Utilization
		CPU0_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_0", O_RDONLY);
		CPU0_UTIL_READ = read(CPU0_UTIL_FILE, CPU0_UTIL_VALUE, sizeof(CPU0_UTIL_VALUE));
		CPU0_UTIL_VALUE[strlen(CPU0_UTIL_VALUE)-1] = '\0';
		/* CPU0_UTIL_READ = read(CPU0_UTIL_FILE, CPU0_UTIL, sizeof(CPU0_UTIL));
		CPU0_UTIL_VALUE = strtod(CPU0_UTIL,NULL); */
		close(CPU0_UTIL_FILE);

		CPU1_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_1", O_RDONLY);
		CPU1_UTIL_READ = read(CPU1_UTIL_FILE, CPU1_UTIL_VALUE, sizeof(CPU1_UTIL_VALUE));
		CPU1_UTIL_VALUE[strlen(CPU1_UTIL_VALUE)-1] = '\0';
		/* CPU1_UTIL_READ = read(CPU1_UTIL_FILE, CPU1_UTIL, sizeof(CPU1_UTIL));
		CPU1_UTIL_VALUE = strtod(CPU1_UTIL,NULL); */
		close(CPU1_UTIL_FILE);

		CPU2_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_2", O_RDONLY);
		CPU2_UTIL_READ = read(CPU2_UTIL_FILE, CPU2_UTIL_VALUE, sizeof(CPU2_UTIL_VALUE));
		CPU2_UTIL_VALUE[strlen(CPU2_UTIL_VALUE)-1] = '\0';
		/* CPU2_UTIL_READ = read(CPU2_UTIL_FILE, CPU2_UTIL, sizeof(CPU2_UTIL));
		CPU2_UTIL_VALUE = strtod(CPU2_UTIL,NULL); */
		close(CPU2_UTIL_FILE);

		CPU3_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_3", O_RDONLY);
		CPU3_UTIL_READ = read(CPU3_UTIL_FILE, CPU3_UTIL_VALUE, sizeof(CPU3_UTIL_VALUE));
		CPU3_UTIL_VALUE[strlen(CPU3_UTIL_VALUE)-1] = '\0';
		/* CPU3_UTIL_READ = read(CPU3_UTIL_FILE, CPU3_UTIL, sizeof(CPU3_UTIL));
		CPU3_UTIL_VALUE = strtod(CPU3_UTIL,NULL); */
		close(CPU3_UTIL_FILE);

		CPU4_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_4", O_RDONLY);
		CPU4_UTIL_READ = read(CPU4_UTIL_FILE, CPU4_UTIL_VALUE, sizeof(CPU4_UTIL_VALUE));
		CPU4_UTIL_VALUE[strlen(CPU4_UTIL_VALUE)-1] = '\0';
		/* CPU4_UTIL_READ = read(CPU4_UTIL_FILE, CPU4_UTIL, sizeof(CPU4_UTIL));
		CPU4_UTIL_VALUE = strtod(CPU4_UTIL,NULL); */
		close(CPU4_UTIL_FILE);

		CPU5_UTIL_FILE = open("/sys/devices/system/cpu/cpufreq/ondemand/cpu_util_5", O_RDONLY);
		CPU5_UTIL_READ = read(CPU5_UTIL_FILE, CPU5_UTIL_VALUE, sizeof(CPU5_UTIL_VALUE));
		CPU5_UTIL_VALUE[strlen(CPU5_UTIL_VALUE)-1] = '\0';
		/* CPU5_UTIL_READ = read(CPU5_UTIL_FILE, CPU5_UTIL, sizeof(CPU5_UTIL));
		CPU5_UTIL_VALUE = strtod(CPU5_UTIL,NULL); */
		close(CPU5_UTIL_FILE);

		// Online/Offline

		CPU0_OF_FILE = open("/sys/devices/system/cpu/cpu0/online", O_RDONLY);
		CPU0_OF_READ = read(CPU0_OF_FILE, CPU0_OF_VALUE, sizeof(CPU0_OF_VALUE));
		CPU0_OF_VALUE[strlen(CPU0_OF_VALUE)-1] = '\0';
		/* CPU0_OF_READ = read(CPU0_OF_FILE, CPU0_OF, sizeof(CPU0_OF));
		CPU0_OF_VALUE = strtod(CPU0_OF,NULL); */
		close(CPU0_OF_FILE);

		CPU1_OF_FILE = open("/sys/devices/system/cpu/cpu1/online", O_RDONLY);
		CPU1_OF_READ = read(CPU1_OF_FILE, CPU1_OF_VALUE, sizeof(CPU1_OF_VALUE));
		CPU1_OF_VALUE[strlen(CPU1_OF_VALUE)-1] = '\0';
		/* CPU1_OF_READ = read(CPU1_OF_FILE, CPU1_OF, sizeof(CPU1_OF));
		CPU1_OF_VALUE = strtod(CPU1_OF,NULL); */
		close(CPU1_OF_FILE);

		CPU2_OF_FILE = open("/sys/devices/system/cpu/cpu2/online", O_RDONLY);
		CPU2_OF_READ = read(CPU2_OF_FILE, CPU2_OF_VALUE, sizeof(CPU2_OF_VALUE));
		CPU2_OF_VALUE[strlen(CPU2_OF_VALUE)-1] = '\0';
		/* CPU2_OF_READ = read(CPU2_OF_FILE, CPU2_OF, sizeof(CPU2_OF));
		CPU2_OF_VALUE = strtod(CPU2_OF,NULL); */
		close(CPU2_OF_FILE);

		CPU3_OF_FILE = open("/sys/devices/system/cpu/cpu3/online", O_RDONLY);
		CPU3_OF_READ = read(CPU3_OF_FILE, CPU3_OF_VALUE, sizeof(CPU3_OF_VALUE));
		CPU3_OF_VALUE[strlen(CPU3_OF_VALUE)-1] = '\0';
		/* CPU3_OF_READ = read(CPU3_OF_FILE, CPU3_OF, sizeof(CPU3_OF));
		CPU3_OF_VALUE = strtod(CPU3_OF,NULL); */
		close(CPU3_OF_FILE);

		CPU4_OF_FILE = open("/sys/devices/system/cpu/cpu4/online", O_RDONLY);
		CPU4_OF_READ = read(CPU4_OF_FILE, CPU4_OF_VALUE, sizeof(CPU4_OF_VALUE));
		CPU4_OF_VALUE[strlen(CPU4_OF_VALUE)-1] = '\0';
		/* CPU4_OF_READ = read(CPU4_OF_FILE, CPU4_OF, sizeof(CPU4_OF));
		CPU4_OF_VALUE = strtod(CPU4_OF,NULL); */
		close(CPU4_OF_FILE);

		CPU5_OF_FILE = open("/sys/devices/system/cpu/cpu5/online", O_RDONLY);
		CPU5_OF_READ = read(CPU5_OF_FILE, CPU5_OF_VALUE, sizeof(CPU5_OF_VALUE));
		CPU5_OF_VALUE[strlen(CPU5_OF_VALUE)-1] = '\0';
		/* CPU5_OF_READ = read(CPU5_OF_FILE, CPU5_OF, sizeof(CPU5_OF));
		CPU5_OF_VALUE = strtod(CPU5_OF,NULL); */
		close(CPU5_OF_FILE);

		//fprintf(fp,"%.5s,%.5s,%.5s,%.5s, %.5s,%.5s,%.5s,%.5s,%.5s, %.5s,%.5s,%.5s,%.5s,%.5s,%.5s,%.5s,%.5s, %s,%s,%s,%s,%s,%s,%s,%s, %.1s,%.1s,%.1s,%.1s,%.1s,%.1s,%.1s,%.1s, %lld,%lld,%lld,%lld,%lld,%lld",
		fprintf(fp,"%s,%s,%s,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%lld",
			CPU_GPU_POWER,SYSTEM_POWER,SOC_POWER,
			microseconds,
			values[0],values[1],values[2], values[3], values[4], values[5],
			TEMP_CPU0_TEMP,TEMP_CPU1_TEMP,TEMP_CPU2_TEMP,TEMP_CPU3_TEMP,TEMP_CPU4_TEMP,TEMP_CPU5_TEMP,
			CPU0_FREQ,
			CPU0_UTIL_VALUE,CPU1_UTIL_VALUE,CPU2_UTIL_VALUE,CPU3_UTIL_VALUE,CPU4_UTIL_VALUE,CPU5_UTIL_VALUE,
			CPU0_OF_VALUE,CPU1_OF_VALUE,CPU2_OF_VALUE,CPU3_OF_VALUE,CPU4_OF_VALUE,CPU5_OF_VALUE,
			tid, timestamp);
					//PC1_VALUE,PC2_VALUE,PC3_VALUE,PC4_VALUE,PC5_VALUE,PC6_VALUE);
					
		//-----------DyPO---------------------------------------------------------------------------------------
		
#ifdef	DYPO
		
			float a7_power=0.0;
			float a15_power=0.0;
			float mem_power=0.0;
			float gpu_power=0.0;
			float total_power=0.0;
			float total_util_a7;
			float total_util_a15;
			int i,j;
			
			float features[NUM_FEATURES]={0},H[NUM_CLASS]={0},H_total=1,Prob[NUM_CLASS]={0},Prob_final=1;
			float H1,H2,H3,H4;
			double H_final;
			float Prob_1,Prob_2,Prob_3,Prob_4,Prob_5;
			
			a7_power = atof(A7_POWER);
			a15_power = atof(A15_POWER);
			mem_power = atof(MEM_POWER);
			gpu_power = atof(GPU_POWER);
			
			total_util_a7 = atof(CPU0_UTIL_VALUE) + atof(CPU1_UTIL_VALUE) + atof(CPU2_UTIL_VALUE) + atof(CPU3_UTIL_VALUE);
			total_util_a15 = atof(CPU4_UTIL_VALUE) + atof(CPU5_UTIL_VALUE) + atof(CPU6_UTIL_VALUE) + atof(CPU7_UTIL_VALUE);
			
			//printf("\n--Utilization: %f, %f",total_util_a7,total_util_a15);
			
			// Calculate total power
			total_power = a15_power + a7_power + mem_power + gpu_power;
			
			for (i=0; i<NUM_FEATURES; i++){
				if (i==0)
					features[i] = 1;
				else if (i==1)
					features[i] = microseconds/1000;
				else if (i==8)
					features[i] = total_power;
				else if (i==9)
					features[i] = total_util_a7;
				else if (i==10)
					features[i] = total_util_a15;
				else
					features[i] = (int)values[i-2]/NORM_PARA;
			}
			
			for (i=0; i<(NUM_CLASS-1); i++){
				for (j=0; j<NUM_FEATURES; j++){
					H[i] += Beta[i][j]*features[j];	
				//	printf("\n--Beta Values: %f--",Beta[i][j]);
				//	printf("\n--Features : %f--",features[j]);
				//	printf("\n--H Values: %f--\n",H[i]);
				}
			}
			
/* 			H1 = BETA_1[0]*1 +
				BETA_1[1]*(microseconds/1000) +				// Convert time in milliseconds
				BETA_1[2]*((int)values[0]/NORM_PARA) +		// Normalize all the performance counters
				BETA_1[3]*((int)values[1]/NORM_PARA) +
				BETA_1[4]*((int)values[2]/NORM_PARA) +
				BETA_1[5]*((int)values[3]/NORM_PARA) +
				BETA_1[6]*((int)values[4]/NORM_PARA) +
				BETA_1[7]*((int)values[5]/NORM_PARA) +
				BETA_1[8]*total_power;
				
			H2 = BETA_2[0]*1 +
				BETA_2[1]*(microseconds/1000) +
				BETA_2[2]*((int)values[0]/NORM_PARA) +
				BETA_2[3]*((int)values[1]/NORM_PARA) +
				BETA_2[4]*((int)values[2]/NORM_PARA) +
				BETA_2[5]*((int)values[3]/NORM_PARA) +
				BETA_2[6]*((int)values[4]/NORM_PARA) +
				BETA_2[7]*((int)values[5]/NORM_PARA) +
				BETA_2[8]*total_power;	
				
			H3 = BETA_3[0]*1 +
				BETA_3[1]*(microseconds/1000) +
				BETA_3[2]*((int)values[0]/NORM_PARA) +
				BETA_3[3]*((int)values[1]/NORM_PARA) +
				BETA_3[4]*((int)values[2]/NORM_PARA) +
				BETA_3[5]*((int)values[3]/NORM_PARA) +
				BETA_3[6]*((int)values[4]/NORM_PARA) +
				BETA_3[7]*((int)values[5]/NORM_PARA) +
				BETA_3[8]*total_power;
			
			H4 = BETA_4[0]*1 +
				BETA_4[1]*(microseconds/1000) +
				BETA_4[2]*((int)values[0]/NORM_PARA) +
				BETA_4[3]*((int)values[1]/NORM_PARA) +
				BETA_4[4]*((int)values[2]/NORM_PARA) +
				BETA_4[5]*((int)values[3]/NORM_PARA) +
				BETA_4[6]*((int)values[4]/NORM_PARA) +
				BETA_4[7]*((int)values[5]/NORM_PARA) +
				BETA_4[8]*total_power;
						
			// Calculate probability
			H_final = 1 + exp(H1) + exp(H2) + exp(H3) + exp(H4); */
			
			for (i=0; i<(NUM_CLASS-1); i++)
				H_total += exp(H[i]);
			
			for (i=0; i<(NUM_CLASS-1); i++){
				Prob[i] = exp(H[i])/H_total;
				Prob_final = Prob_final - Prob[i];
			}
			
			//printf("\n--%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f----\n",features[0],features[1],features[2],features[3],features[4],features[5],features[6],features[7],features[8],features[9],features[10],H[0],H[1],H[2],H[3],H[4],Prob[0],Prob[1],Prob[2],Prob[3],Prob[4]);
			
			
			if (Prob[0] > 0.5f){
				change_min_A15_freq(800000);
				change_max_A15_freq(800000);
				change_min_A7_freq(800000);
				change_max_A7_freq(800000);
			} else if (Prob[1] > 0.5f){
				change_min_A15_freq(1000000);
				change_max_A15_freq(1000000);
				change_min_A7_freq(1000000);
				change_max_A7_freq(1000000);				
			} else if (Prob[2] > 0.5f){
				change_min_A15_freq(1200000);
				change_max_A15_freq(1200000);
				change_min_A7_freq(1200000);
				change_max_A7_freq(1200000);				
			} else if (Prob[3] > 0.5f){
				change_min_A15_freq(1400000);
				change_max_A15_freq(1400000);
				change_min_A7_freq(1400000);
				change_max_A7_freq(1400000);				
			} else if (Prob[4] > 0.5f){
				change_min_A15_freq(1600000);
				change_max_A15_freq(1600000);
				change_min_A7_freq(1400000);
				change_max_A7_freq(1400000);				
			} else if (Prob_final > 0.5f){
				change_min_A15_freq(2000000);
				change_max_A15_freq(2000000);
				change_min_A7_freq(1400000);
				change_max_A7_freq(1400000);
			}
			
/*  			Prob_1 = exp(H1)/H_final;
			Prob_2 = exp(H2)/H_final;
			Prob_3 = exp(H3)/H_final;
			Prob_4 = exp(H4)/H_final; 
			Prob_5 = 1 - Prob_1 - Prob_2 - Prob_3 - Prob_4; */
			
 			//printf("\n--%lld,%lld,%lld,%lld,%lld,%lld,%lld,%f,%f,%f,%f,%f,%f,%f,%f,%f----\n",(microseconds/1000),values[0]/NORM_PARA,values[1]/NORM_PARA,values[2]/NORM_PARA,values[3]/NORM_PARA,values[4]/NORM_PARA,values[5]/NORM_PARA,total_power,H[0],H[1],H[2],H[3],Prob[0],Prob[1],Prob[2],Prob[3]);
			//printf("\n--%f,%f,%f,%f,%f,%f,%f,%f----\n",H1,H2,H3,H4,Prob_1,Prob_2,Prob_3,Prob_4);
			//printf("\n-POWER-%f,%f,%f,%f",a15_power,a7_power,mem_power,gpu_power); */		
			//printf("\n--%f,%f,%f,%f,%f,%f,%f,%f----\n",H[0],H[1],H[2],H[3],Prob[0],Prob[1],Prob[2],Prob[3]);
			
/* 			// Total number of desired active cores
			int des_a7=0;		
			int des_a15=0;
			
			// Threshold values to make core configuration decision 
			float a7_util_thld=50.0;
			float a15_util_thld=50.0;
			
			// online offline and active core vector
			int online_offline_vector[8];
			int active_core_vector[8];
			
			// Temporary sorting variable
			int a=0,n=8;
			
			// Core utilization
			float core_util_a7[4];
			float core_util_a15[4];
			
			//core_util_a7[0]=atof(CPU0_UTIL_VALUE);
			core_util_a7[0]=atof(CPU1_UTIL_VALUE);
			core_util_a7[1]=atof(CPU2_UTIL_VALUE);
			core_util_a7[2]=atof(CPU3_UTIL_VALUE);
			
			core_util_a15[0]=atof(CPU4_UTIL_VALUE);
			core_util_a15[1]=atof(CPU5_UTIL_VALUE);
			core_util_a15[2]=atof(CPU6_UTIL_VALUE);
			core_util_a15[3]=atof(CPU7_UTIL_VALUE);
			
			// Sort the core utilization
			int size_a7 = 3;
			int size_a15 = 4;
			int index_a7[size_a7];
			int index_a15[size_a15];
			
			for(i=0;i<size_a7;i++){
				index_a7[i] = i;
			}
			
			for(i=0;i<size_a15;i++){
				index_a15[i] = i;
			}
			
			array = core_util_a7;
			qsort(index_a7, size_a7, sizeof(*index_a7), cmp);
			
			array = core_util_a15;
			qsort(index_a15, size_a15, sizeof(*index_a15), cmp);
					
			// Set core configuration based on probability values
			if (Prob[0] > 0.5f){			
				
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}
							
			} else if (Prob[1] > 0.5f){
				
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}
				
			} else if (Prob[2] > 0.5f){
				
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,1);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}
				
			} else if (Prob[3] > 0.5f){
							
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,1);
				change_online_offline_cores(index_a7[2] + 1,1);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}
										
			} else if (Prob[4] > 0.5f){
		
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}	
				
			} else if (Prob[5] > 0.5f){
				
				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}	
								
			} else if (Prob[6] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}									

			} else if (Prob[7] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,1);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}									

			} else if (Prob[8] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}									

			} 	else if (Prob[9] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}										

			} 	else if (Prob[10] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}									

			} 	else if (Prob[11] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,1);
				change_online_offline_cores(index_a7[2] + 1,1);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,0);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}								

			} 	else if (Prob[12] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,0);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}										

			} 	else if (Prob[13] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,1);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}										

			} 	else if (Prob[14] > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,0);
				change_online_offline_cores(index_a7[1] + 1,0);
				change_online_offline_cores(index_a7[2] + 1,0);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,0);
				change_online_offline_cores(index_a15[1] + 4,1);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}										

			} 	else if (Prob_final > 0.5f){

				// Decision for A7
				change_online_offline_cores(0,1);
				change_online_offline_cores(index_a7[0] + 1,1);
				change_online_offline_cores(index_a7[1] + 1,1);
				change_online_offline_cores(index_a7[2] + 1,1);
				
				// Decision for A15
				change_online_offline_cores(index_a15[0] + 4,1);
				change_online_offline_cores(index_a15[1] + 4,1);
				change_online_offline_cores(index_a15[2] + 4,1);
				change_online_offline_cores(index_a15[3] + 4,1);
			
				for(i=0;i<size_a7;i++){
					index_a7[i] = 0;
				}
			
				for(i=0;i<size_a15;i++){
					index_a15[i] = 0;
				}								

			}  */		

#endif
		
	fclose(fp);
}
