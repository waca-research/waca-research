/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sha.h"

//################################################################
// Start of PAPI related variables
//################################################################
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log_data.h"
#include <pthread.h>
  
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

int retval;
int EventSet = PAPI_NULL;
int EventCode;
int skip_papi_cntr = 0;
int skip_papi_cntr_threshold = 20;
char EventCodeStr[PAPI_MAX_STR_LEN];
long_long values[35];

struct timespec begin;
struct timespec current;
long long start;
long long elapsed;
long long microseconds;
long long previoustime = 0;

static int NUM_THREADS = -1;
pthread_mutex_t mutex_papi = PTHREAD_MUTEX_INITIALIZER;

//################################################################
// End of PAPI related variables
//################################################################

#define NANOS 1000000000LL

int main(int argc, char **argv)
{


	//---- PAPI----------------------------------------------------------------------------------------------------

  	// Start the timer
	if (clock_gettime(CLOCK_MONOTONIC , &begin)) {
        /* Oops, getting clock time failed */
        exit(EXIT_FAILURE);
    }
	/* The printf's may be removed to isolate just the math calculations */


	/* Start time in nanoseconds */
    start = begin.tv_sec*NANOS + begin.tv_nsec;

	// Init PAPI
	papi_init();

	//---- END OF PAPI----------------------------------------------------------------------------------------------------



    FILE *fin;
    SHA_INFO sha_info;

    if (argc < 2) {
		fin = stdin;
		sha_stream(&sha_info, fin);
		sha_print(&sha_info);
	    } else {
		while (--argc) {
		    fin = fopen(*(++argv), "rb");
		    if (fin == NULL) {
			printf("error opening %s for reading\n", *argv);
		    } else {
			sha_stream(&sha_info, fin);
			sha_print(&sha_info);
			fclose(fin);
		    }
		}
    }
    waca_papi_end("Hello");   	//-----------WACA: Read PAPI at end--------------------
    return(0);
}
