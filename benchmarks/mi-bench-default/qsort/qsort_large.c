#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define UNLIMIT
#define MAXARRAY 60000 /* this number, if too large, will cause a seg. fault!! */

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
int skip_papi_cntr_threshold = 1500;
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


struct my3DVertexStruct {
  int x, y, z;
  double distance;
};

int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;

  distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
  distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}


int main(int argc, char *argv[]) {



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

  struct my3DVertexStruct array[MAXARRAY];
  FILE *fp;
  int i,count=0;
  int x, y, z;
  
  if (argc<2) {
    fprintf(stderr,"Usage: qsort_large <file>\n");
    exit(-1);
  }
  else {
    fp = fopen(argv[1],"r");
    
    while((fscanf(fp, "%d", &x) == 1) && (fscanf(fp, "%d", &y) == 1) && (fscanf(fp, "%d", &z) == 1) &&  (count < MAXARRAY)) {
	 array[count].x = x;
	 array[count].y = y;
	 array[count].z = z;
	 array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	 count++;
	 waca_papi_read("Hello");		// -----------PAPI---------------
    }
  }
  printf("\nSorting %d vectors based on distance from the origin.\n\n",count);
  qsort(array,count,sizeof(struct my3DVertexStruct),compare);

  //printf("PAPI_read call after qsort\n");
  
  //waca_papi_read("Hello"); 	// -----------PAPI---------------
  
  for(i=0;i<count;i++)
    printf("%d %d %d\n", array[i].x, array[i].y, array[i].z);
	
	waca_papi_end("Hello");   	//-----------WACA: Read PAPI at end--------------------
  
  return 0;

}
