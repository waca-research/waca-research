#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "log_data.h"

extern int retval;
extern int EventSet;
extern int EventCode;
extern int skip_papi_cntr;
extern int skip_papi_cntr_threshold;
extern char EventCodeStr[PAPI_MAX_STR_LEN];
extern long_long values[35];

extern struct timespec begin;
extern struct timespec current;
extern long long start;
extern long long elapsed;
extern long long microseconds;
extern long long previoustime;

extern pthread_mutex_t mutex_papi;

#define NANOS 1000000000LL

// Read PAPI in intervals
int waca_papi_read (const char *format, ...)
{ 

	// Store temparory value of timestamp
	long long microseconds_temp = 0;
        long long timestamp;
	skip_papi_cntr++;
	
	if (skip_papi_cntr == skip_papi_cntr_threshold) {

		// Get elapsed time
		if (clock_gettime(CLOCK_MONOTONIC , &current)) {
			/* getting clock time failed, what now? */
			exit(EXIT_FAILURE);
		}
		
		// Read the PAPI counters
		if (PAPI_read(EventSet, values) != PAPI_OK) 
			printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

		// Elapsed time in nanoseconds
		elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
		microseconds_temp = elapsed / 1000 + (elapsed % 1000 >= 500); // round up halves
		timestamp = microseconds_temp;

		// Previous time
		microseconds = current.tv_sec*NANOS + current.tv_nsec - previoustime;
		microseconds = microseconds / 1000 + (microseconds % 1000 >= 500);

		// Logging performance data
		log_data(values, 0, microseconds, timestamp);

		// Reset PAPI counters
		if (PAPI_reset(EventSet) != PAPI_OK) 
		    printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);	// If PAPI_reset returns fales the print error

		skip_papi_cntr = 0;

  	        // Start the timer
	        if (clock_gettime(CLOCK_MONOTONIC , &current)) {
                    exit(EXIT_FAILURE);
                }

	        // Start time in nanoseconds
                previoustime = current.tv_sec*NANOS + current.tv_nsec;

	}

	return 1;
}

// Read PAPI at the end
int waca_papi_end (const char *format, ...)
{ 

	// Store temparory value of timestamp
	long long microseconds_temp = 0;
        long long timestamp;

	if (PAPI_read(EventSet, values) != PAPI_OK) 
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Get elapsed time
	if (clock_gettime(CLOCK_MONOTONIC , &current)) {
		/* getting clock time failed, what now? */
		exit(EXIT_FAILURE);
	}
	
	// Elapsed time in nanoseconds
	elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
	microseconds_temp = elapsed / 1000 + (elapsed % 1000 >= 500); // round up halves
	timestamp = microseconds_temp;

	// Previous time
	microseconds = current.tv_sec*NANOS + current.tv_nsec - previoustime;
	microseconds = microseconds / 1000 + (microseconds % 1000 >= 500);

	// Logging performance data
	log_data(values, 0, microseconds, timestamp);

	return 1;
}

// Initialize PAPI 
void papi_init(){
	retval = PAPI_library_init(PAPI_VER_CURRENT);

	if (retval != PAPI_VER_CURRENT) {
		fprintf(stderr, "PAPI library init error!\n");
		exit(1);
	}

	if (PAPI_create_eventset(&EventSet) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Instructions
	if ( PAPI_event_name_to_code( "PAPI_TOT_INS", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// CPU Cycles
	if ( PAPI_event_name_to_code( "PAPI_TOT_CYC", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Branch Miss Prediction
	if ( PAPI_event_name_to_code( "PAPI_BR_MSP", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Level 2 cache misses
	if ( PAPI_event_name_to_code( "PAPI_L2_DCM", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Data Memory Access
	if ( PAPI_event_name_to_code( "PAPI_L2_DCA", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// NONCACHE_EXTERNAL_MEMORY_REQUEST
	if ( PAPI_event_name_to_code( "PAPI_L1_ICM", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Start PAPI calls.
	if (PAPI_start(EventSet) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

  	// Start the timer
	if (clock_gettime(CLOCK_MONOTONIC , &current)) {
            exit(EXIT_FAILURE);
        }

	// Start time in nanoseconds
        previoustime = current.tv_sec*NANOS + current.tv_nsec;

}

int mt_waca_papi_read(int EventSet, long long values[], int tid, long long *thread_start, int *thread_papi_skip_counter) {

	long long timestamp_temp = 0;
        long long exe_time = 0;
        long long timestamp_curr = 0;
	
	*thread_papi_skip_counter = *thread_papi_skip_counter + 1;
	
	if (*thread_papi_skip_counter == skip_papi_cntr_threshold) {

	// Get elapsed time
	if (clock_gettime(CLOCK_MONOTONIC , &current)) {
		/* getting clock time failed, what now? */
		exit(EXIT_FAILURE);
	}

	// Stop PAPI
	if (PAPI_stop(EventSet, values) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
	timestamp_temp = current.tv_sec*NANOS + current.tv_nsec - start;
	timestamp_curr = timestamp_temp / 1000 + (timestamp_temp % 1000 >= 500); // round up halves

	exe_time = current.tv_sec*NANOS + current.tv_nsec - *thread_start;
	exe_time = exe_time / 1000 + (exe_time % 1000 >= 500); // round up halves

	// Lock mutex
	pthread_mutex_lock(&mutex_papi);
	
	// Logging performance data
	log_data(values, (tid+1), exe_time, timestamp_curr);
	
	// Unlock mutex
	pthread_mutex_unlock(&mutex_papi);

	*thread_papi_skip_counter = 0;

	// Start PAPI calls.
	if (PAPI_start(EventSet) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

        // Start the timer
        if (clock_gettime(CLOCK_MONOTONIC , &current)) {
            exit(EXIT_FAILURE);
        }

        // Start time in nanoseconds
        *thread_start = current.tv_sec*NANOS + current.tv_nsec;

	}
	
	return 0;
}

