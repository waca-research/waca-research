#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

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
int skip_papi_cntr_threshold = 1;
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


double SIGMA = 0.1;
double DIV_FACTOR = 500;

double distance(double *p1, double *p2, int m)
{
	int i;
	double dist = 0;
	//find euclidean distance
	for(i=0; i<m; i++)
		dist += (p2[i] - p1[i])*(p2[i] - p1[i]);

	return dist;
}

void adjacency(double **data, int n, int m, double *A)
{
	int i=0, j=0;
	for(i=0; i<n; i++) {

			// ---------PAPI-----------
		if(i % 4 == 0 ) {
		    waca_papi_read("Hello");
		}
		    // ---------PAPI-----------
		for(j=i;j<n;j++){
			if(i==j) A[(n*i+j)] = 0;
			else
				A[(n*i+j)] = -exp(-distance(data[i], data[j], m)); // Gauss Kernel
			A[(n*j+i)] = A[(n*i+j)];
		}
	}
}


void get_k_data(double *A, double *ev, int n, int k, double **k_data)
{
	int i, j, l;
	int index = 0;
	double min = DBL_MAX;

	int *check = calloc(n, sizeof(int));

	for(i=0; i<k; i++)
	{
		min = DBL_MAX;
		for(j=0; j<n; j++)
		{
			if(check[j]) continue;
			else {
				if(ev[j]<min) {
					min = ev[j];
					index = j;
				}
			}
		}
		check[index] = 1;
		for(l=0;l<n;l++) 
			k_data[l][i] = A[l*n+index];
	}
}

void get_data(const char* data_file, int n, int m, double **data)
{
	int i, j;
	FILE *fp = fopen(data_file,"r");

	for(i=0; i<n; i++)
	{
			// ---------PAPI-----------
		if(i % 8 == 0 ) {  //--- 512: Gives 400,000 instruction gap between PAPI Reads
		    waca_papi_read("Hello");
		}
		    // ---------PAPI-----------
		for(j=0; j<m; j++)
			fscanf(fp, "%lf", &data[i][j]);
	}
}

void USAGE()
{
	printf("USAGE: \n");
	printf("./spc data_file n m k SIGMA DIV_FACTOR\n");
	printf("data_file = dataset text file in the format specified below \n");
	printf("n = total number of data points \n");
	printf("m = dimensionality of data \n");
	printf("k = number of desired clusters\n");
	printf("SIGMA = Gauss Kernel Sigma (typically set it to 0.707)\n");
	printf("DIV_FACTOR = Factor to avoid overflowing. Might require for Data-points \n");
	printf("very far away or with very high abs value (typically set it to 1) \n");
	printf("\n");
	printf("**** data_file format ****\n");
	printf("data_file should be in the form nxm\n");
	printf("n = number of rows (total number of data points)\n");
	printf("m = number of columns (data point dimensions)\n");
}

int main(int argc, char *argv[])
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

	char data_file[1000] = "./datasets/spiral";
	int n = 312, m = 2, k=3;
	int i, j;
	unsigned int* start, *stop, *elapsed;

//	start = photonStartTiming();
	if(argc != 7) {
		USAGE();
		return 0;
	}
	else {
		strcpy(data_file, (const char*)argv[1]);
		n = atoi(argv[2]);
		m = atoi(argv[3]);
		k = atoi(argv[4]);
		SIGMA = atof(argv[5]);
		DIV_FACTOR = atof(argv[6]);
	}

	printf("n = %d, m = %d, k = %d SIGMA = %f DIV_FACTOR = %f \n", n, m, k, SIGMA, DIV_FACTOR);

	//alloc data
	double **data = calloc(n, sizeof(double *));
	for(i=0; i<n; i++)
	{
		data[i] = calloc(m, sizeof(double));
	}

	//alloc Adjacency/Laplacian Matrix
	double *A  = calloc(n*n, sizeof(double));
	if(!A) { 
		printf("Data Set too large. Could not allocate memory \n");
		return 1;
	}

	//alloc degree matrix
	double *D = calloc(n, sizeof(double));

	//eigen vectors
	double *ev = calloc(n, sizeof(double));

	//k_data
	double **k_data = calloc(n, sizeof(double *));
	for(i=0; i<n; i++)
	{
		k_data[i] = calloc(k, sizeof(double));
	}

	//get data
	get_data(data_file, n, m, data);

	//find adjacency matrix
	adjacency(data, n, m, A);



	//Calculate Laplacian matrix (degree - adjacency)
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			D[i] += -A[(n*i+j)];
		A[(n*i+i)] = D[i];
	}

	//Second level Laplacian
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			A[(n*i+j)] = (A[(n*i+j)]/sqrt(D[i]))/sqrt(D[j]);
	}


	// ---------PAPI-----------
    waca_papi_read("Hello");
    // ---------PAPI-----------

	eigen(A, ev, n);


	// ---------PAPI-----------
    waca_papi_read("Hello");
    // ---------PAPI-----------

	//find eigen values that are close to zero and corresponding eigen vectors.
	get_k_data(A, ev, n, k, k_data);


	// ---------PAPI-----------
    waca_papi_read("Hello");
    // ---------PAPI-----------

	//peform k-means on each of these eigen vectors
	int *c = k_means(k_data, n, k, k, 1e-8, 0);

		// ---------PAPI-----------
    waca_papi_read("Hello");
    // ---------PAPI----------

	//for (i = 0; i < n; i++) {
	//	printf("data point %d is in cluster %d\n", i, c[i]);
	//}

//	stop = photonEndTiming();
//	elapsed = photonReportTiming(start,stop);
//	photonPrintTiming(elapsed);

   /****
   ** housekeeping */
	if(!c) free(c);
	if(!ev) free(ev);
	if(!D) free(D);
	if(!A) free(A);

	for(i=0; i<n; i++) {
		free(k_data[i]);
		free(data[i]);
	}
	if(!k_data) free(k_data);
	if(!data) free(data);
	
	waca_papi_end("Hello");   	//-----------WACA: Read PAPI at end--------------------
	
	return 0;
}
