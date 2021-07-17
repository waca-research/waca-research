/* testc - Test adpcm coder */

#include "adpcm.h"
#include <stdio.h>

//################################################################
// Start of PAPI related variables
//################################################################
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "log_data.h"
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
int skip_papi_cntr_threshold = 200;
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


struct adpcm_state state;

#define NSAMPLES 1000

char	abuf[NSAMPLES/2];
short	sbuf[NSAMPLES];

int main() {
    int n;


//---- PAPI----------------------------------------------------------------------------------------------------

  	// Start the timer
	if (clock_gettime(CLOCK_MONOTONIC , &begin)) {
        /* Oops, getting clock time failed */
        exit(EXIT_FAILURE);
    }

	/* Start time in nanoseconds */
    start = begin.tv_sec*NANOS + begin.tv_nsec;

	// Init PAPI
	papi_init();

//---- END OF PAPI----------------------------------------------------------------------------------------------------



    while(1) {
		n = read(0, sbuf, NSAMPLES*2);
		if ( n < 0 ) {
		    perror("input file");
		    exit(1);
		}
		if ( n == 0 ) break;
		adpcm_coder(sbuf, abuf, n/2, &state);

		// ---------PAPI-----------
	    waca_papi_read("Hello");
	    // ---------PAPI-----------

		write(1, abuf, n/4);
    }
    fprintf(stderr, "Final valprev=%d, index=%d\n",
	    state.valprev, state.index);

    waca_papi_end("Hello");   	//-----------WACA: Read PAPI at end--------------------
    
    exit(0);
}
