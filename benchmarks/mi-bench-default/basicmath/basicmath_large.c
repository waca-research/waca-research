#include "snipmath.h"
#include <math.h>

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
int skip_papi_cntr_threshold = 2500;
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


int main(void)
{

//---- PAPI----------------------------------------------------------------------------------------------------

  	// Start the timer
	if (clock_gettime(CLOCK_MONOTONIC , &begin)) {
        exit(EXIT_FAILURE);
    }

	// Start time in nanoseconds
    start = begin.tv_sec*NANOS + begin.tv_nsec;

	// Init PAPI
	papi_init();

//---- END OF PAPI----------------------------------------------------------------------------------------------------

  double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
  double  x[3];
  double X;
  int     solutions;
  int i;
  unsigned long l = 0x3fed0169L;
  struct int_sqrt q;
  long n = 0;
  /* solve soem cubic functions */
  printf("********* CUBIC FUNCTIONS ***********\n");
  /* should get 3 solutions: 2, 6 & 2.5   */
  SolveCubic(a1, b1, c1, d1, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = 1.0; b1 = -4.5; c1 = 17.0; d1 = -30.0;
  /* should get 1 solution: 2.5           */
  SolveCubic(a1, b1, c1, d1, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = 1.0; b1 = -3.5; c1 = 22.0; d1 = -31.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = 1.0; b1 = -13.7; c1 = 1.0; d1 = -35.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = 3.0; b1 = 12.34; c1 = 5.0; d1 = 12.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = -8.0; b1 = -67.89; c1 = 6.0; d1 = -23.6;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = 45.0; b1 = 8.67; c1 = 7.5; d1 = 34.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  a1 = -12.0; b1 = -1.7; c1 = 5.3; d1 = 16.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  
  
  /* Now solve some random equations */
  for(a1=1;a1<10;a1+=1) {
    for(b1=10;b1>0;b1-=.25) {
      for(c1=5;c1<15;c1+=0.61) {
	   for(d1=-1;d1>-5;d1-=.451) {
		SolveCubic(a1, b1, c1, d1, &solutions, x);  
		printf("Solutions:");
		for(i=0;i<solutions;i++)
		  printf(" %f",x[i]);
		printf("\n");
	   }
      }
    }
  }

  printf("********* INTEGER SQR ROOTS ***********\n");
  /* perform some integer square roots */
  for (i = 0; i < 100000; i+=2)
    {
      usqrt(i, &q);
			// remainder differs on some machines
     // printf("sqrt(%3d) = %2d, remainder = %2d\n",
     printf("sqrt(%3d) = %2d\n",
	     i, q.sqrt);
    }

  printf("\n");
  for (l = 0x3fed0169L; l < 0x3fed4169L; l++)
    {
	 usqrt(l, &q);
	 //printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);
	 printf("sqrt(%lX) = %X\n", l, q.sqrt);
    }
    

  printf("********* ANGLE CONVERSION ***********\n");
  /* convert some rads to degrees */
/*   for (X = 0.0; X <= 360.0; X += 1.0) */
  for (X = 0.0; X <= 360.0; X += .001)
    printf("%3.0f degrees = %.12f radians\n", X, deg2rad(X));
    puts("");
/*   for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180)) */

  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 5760))
    printf("%.12f radians = %3.0f degrees\n", X, rad2deg(X));
  
	//-----------WACA: Read PAPI at end--------------------
	waca_papi_end("Hello");
  
  return 0;
}
