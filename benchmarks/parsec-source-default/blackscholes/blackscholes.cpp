// Copyright (c) 2007 Intel Corp.

// Black-Scholes
// Analytical method for calculating European Options
//
// 
// Reference Source: Options, Futures, and Other Derivatives, 3rd Edition, Prentice 
// Hall, John C. Hull,

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif

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


// Multi-threaded pthreads header
#ifdef ENABLE_THREADS
// Add the following line so that icc 9.0 is compatible with pthread lib.
#define __thread __threadp
MAIN_ENV
#undef __thread
#endif

// Multi-threaded OpenMP header
#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

#ifdef ENABLE_TBB
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

using namespace std;
using namespace tbb;
#endif //ENABLE_TBB

// Multi-threaded header for Windows
#ifdef WIN32
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
#include <windows.h>
#endif

//Precision to use for calculations
#define fptype float
#define NUM_RUNS 100


typedef struct OptionData_ {
        fptype s;          // spot price
        fptype strike;     // strike price
        fptype r;          // risk-free interest rate
        fptype divq;       // dividend rate
        fptype v;          // volatility
        fptype t;          // time to maturity or option expiration in years 
                           //     (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)  
        char OptionType;   // Option type.  "P"=PUT, "C"=CALL
        fptype divs;       // dividend vals (not used in this test)
        fptype DGrefval;   // DerivaGem Reference Value
} OptionData;

OptionData *data;
fptype *prices;
int numOptions;

int    * otype;
fptype * sptprice;
fptype * strike;
fptype * rate;
fptype * volatility;
fptype * otime;
int numError = 0;
int nThreads;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
#define inv_sqrt_2xPI 0.39894228040143270286

fptype CNDF ( fptype InputX ) 
{
    int sign;

    fptype OutputX;
    fptype xInput;
    fptype xNPrimeofX;
    fptype expValues;
    fptype xK2;
    fptype xK2_2, xK2_3;
    fptype xK2_4, xK2_5;
    fptype xLocal, xLocal_1;
    fptype xLocal_2, xLocal_3;

    // Check for negative value of InputX
    if (InputX < 0.0) {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;

    xInput = InputX;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    expValues = exp(-0.5f * InputX * InputX);
    xNPrimeofX = expValues;
    xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;
	
    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;
    
    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782);
    xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;

    xLocal_1 = xLocal_2 + xLocal_1;
    xLocal   = xLocal_1 * xNPrimeofX;
    xLocal   = 1.0 - xLocal;

    OutputX  = xLocal;
    
    if (sign) {
        OutputX = 1.0 - OutputX;
    }
    
    return OutputX;
} 

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
fptype BlkSchlsEqEuroNoDiv( fptype sptprice,
                            fptype strike, fptype rate, fptype volatility,
                            fptype time, int otype, float timet )
{
    fptype OptionPrice;

    // local private working variables for the calculation
    fptype xStockPrice;
    fptype xStrikePrice;
    fptype xRiskFreeRate;
    fptype xVolatility;
    fptype xTime;
    fptype xSqrtTime;

    fptype logValues;
    fptype xLogTerm;
    fptype xD1; 
    fptype xD2;
    fptype xPowerTerm;
    fptype xDen;
    fptype d1;
    fptype d2;
    fptype FutureValueX;
    fptype NofXd1;
    fptype NofXd2;
    fptype NegNofXd1;
    fptype NegNofXd2;    
    
    xStockPrice = sptprice;
    xStrikePrice = strike;
    xRiskFreeRate = rate;
    xVolatility = volatility;

    xTime = time;
    xSqrtTime = sqrt(xTime);

    logValues = log( sptprice / strike );
        
    xLogTerm = logValues;
        
    
    xPowerTerm = xVolatility * xVolatility;
    xPowerTerm = xPowerTerm * 0.5;
        
    xD1 = xRiskFreeRate + xPowerTerm;
    xD1 = xD1 * xTime;
    xD1 = xD1 + xLogTerm;

    xDen = xVolatility * xSqrtTime;
    xD1 = xD1 / xDen;
    xD2 = xD1 -  xDen;

    d1 = xD1;
    d2 = xD2;
    
    NofXd1 = CNDF( d1 );   
	NofXd2 = CNDF( d2 );
	
    FutureValueX = strike * ( exp( -(rate)*(time) ) );        
    if (otype == 0) {            
        OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
    } else { 
        NegNofXd1 = (1.0 - NofXd1);
        NegNofXd2 = (1.0 - NofXd2);
        OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
    }
    
    return OptionPrice;
}

#ifdef ENABLE_TBB
struct mainWork {
  mainWork() {}
  mainWork(mainWork &w, tbb::split) {}

  void operator()(const tbb::blocked_range<int> &range) const {
    fptype price;
    int begin = range.begin();
    int end = range.end();

    for (int i=begin; i!=end; i++) {
      /* Calling main function to calculate option value based on 
       * Black & Scholes's equation.
       */

      price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                   rate[i], volatility[i], otime[i], 
                                   otype[i], 0);
      prices[i] = price;

#ifdef ERR_CHK 
      fptype priceDelta = data[i].DGrefval - price;
      if( fabs(priceDelta) >= 1e-5 ){
        fprintf(stderr,"Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
               i, price, data[i].DGrefval, priceDelta);
        numError ++;
      }
#endif
    }
  }
};

#endif // ENABLE_TBB

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_TBB
int bs_thread(void *tid_ptr) {
	
	// PAPI read
	waca_papi_read("Hello");
	
    int j;
    tbb::affinity_partitioner a;

    mainWork doall;
    for (j=0; j<NUM_RUNS; j++) {
      tbb::parallel_for(tbb::blocked_range<int>(0, numOptions), doall, a);
    }

    return 0;
}
#else // !ENABLE_TBB

#ifdef WIN32
DWORD WINAPI bs_thread(LPVOID tid_ptr){
#else
int bs_thread(void *tid_ptr) {
#endif
    int i, j;
    fptype price;
    fptype priceDelta;
    int tid = *(int *)tid_ptr;
    int start_tid = tid * (numOptions / nThreads);
    int end = start_tid + (numOptions / nThreads);
	
//-----------------------------------------MULTI - PAPI----------------------------------------------------------------------------------------------------
	
 	int retval,EventSet = PAPI_NULL, EventCode;
 	struct timespec mt_begin, mt_current;

	// Elapsed timestamp
	//long long mt_elapsed[4], mt_timestamp[4]; //Hard coded 4 value has to be romved to get total number of threads
	//static long long mt_microseconds[4];

	/* if (clock_gettime(CLOCK_MONOTONIC , &current)) {
			// getting clock time failed, what now?
			exit(EXIT_FAILURE);
	}
	start_time = current.tv_sec*NANOS + current.tv_nsec; */
	
	//long long microseconds=0;
	
	// Include header file that includes functions to thread related PAPI functions
	#ifdef xavier_nx
	  #include "mt_papi_init_xavier_nx.h"
	#elif agx_xavier
	  #include "mt_papi_init_agx_xavier.h"
	#elif odroid
	  #include "mt_papi_init_odroid.h"
	#endif

// #ifdef SINGLE_PAPI
   
// 	// Initialize multiple threads
// 	pthread_mutex_t mutex_papi = PTHREAD_MUTEX_INITIALIZER;
	

// 	// Register thread
// 	retval = PAPI_register_thread(  );
// 	if ( retval != PAPI_OK )
// 		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

// 	// Start PAPI calls.
// 	if (PAPI_start(EventSet) != PAPI_OK)
// 		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
// #endif // SINGLE_PAPI 
//-----------------------------------------MULTI - PAPI----------------------------------------------------------------------------------------------------
// current time 1, prev time is 0
    for (j=0; j<NUM_RUNS; j++) {
		
		// -----------------------------------------MULTI- PAPI--Capture current time------------
			// microseconds = PAPI_get_real_usec(  );

  //   	if (clock_gettime(CLOCK_MONOTONIC , &current)) {
		// 	// getting clock time failed, what now?
		// 	exit(EXIT_FAILURE);
		// 	}
	
		// 	//Elapsed time in nanoseconds
		// mt_elapsed[tid] = current.tv_sec*NANOS + current.tv_nsec - start_time;
		// mt_microseconds[tid] = mt_elapsed[tid] / 1000 + (mt_elapsed[tid] % 1000 >= 500); // round up halves 

		
#ifdef ENABLE_OPENMP
#pragma omp parallel for private(i, price, priceDelta)
        for (i=0; i<numOptions; i++) {
#else  //ENABLE_OPENMP
	        for (i=start_tid; i<end; i++) {
#endif //ENABLE_OPENMP
	            /* Calling main function to calculate option value based on 
	             * Black & Scholes's equation.
	             */
	            price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
	                                         rate[i], volatility[i], otime[i], 
	                                         otype[i], 0);
	            prices[i] = price;

#ifdef ERR_CHK
	            priceDelta = data[i].DGrefval - price;
	            if( fabs(priceDelta) >= 1e-4 ){
	                printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
	                       i, price, data[i].DGrefval, priceDelta);
	                numError ++;
	            } // if( fabs(priceDelta) >= 1e-4 ){
#endif
	        } // for (i=start; i<end; i++) OR for (i=0; i<numOptions; i++)
				
//-----------------------------------------MULTI - PAPI----------------------------------------------------------------------------------------------------
	//mt_timestamp[tid] = mt_waca_papi_read(EventSet,values,tid,mt_timestamp[tid], start_time);		
	mt_waca_papi_read(EventSet,values,tid,&thread_start);
		
    } // for (j=0; j<NUM_RUNS; j++)

	
// -----------------------------------------MULTI - PAPI----------------------------------------------------------------------------------------------------

	// Deregister thread
	retval = PAPI_unregister_thread(  );
	if ( retval != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
 	
//-----------------------------------------PAPI----------------------------------------------------------------------------------------------------

    return 0;
}
#endif //ENABLE_TBB

int main (int argc, char **argv)
{
	

#ifdef SINGLE_PAPI
//---- PAPI----------------------------------------------------------------------------------------------------

  	// Start the timer
	if (clock_gettime(CLOCK_MONOTONIC , &begin)) {
        exit(EXIT_FAILURE);
    }

	// Start time in nanoseconds
    waca_start_time = begin.tv_sec*NANOS + begin.tv_nsec;

	// Init PAPI
	papi_init();
#endif // SINGLE_PAPI

//---- END OF PAPI----------------------------------------------------------------------------------------------------
	
    FILE *file;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;

#ifdef PARSEC_VERSION
#define __PARSEC_STRING(x) #x
#define __PARSEC_XSTRING(x) __PARSEC_STRING(x)
        printf("PARSEC Benchmark Suite Version "__PARSEC_XSTRING(PARSEC_VERSION)"\n");
	fflush(NULL);
#else
        printf("PARSEC Benchmark Suite\n");
	fflush(NULL);
#endif //PARSEC_VERSION
#ifdef ENABLE_PARSEC_HOOKS
   __parsec_bench_begin(__parsec_blackscholes);
#endif

   if (argc != 4)
        {
                printf("Usage:\n\t%s <nthreads> <inputFile> <outputFile>\n", argv[0]);
				
                exit(1);
        }
    nThreads = atoi(argv[1]);
	NUM_THREADS = nThreads;
    char *inputFile = argv[2];
    char *outputFile = argv[3];

    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }
    if(nThreads > numOptions) {
      printf("WARNING: Not enough work, reducing number of threads to match number of options.\n");
      nThreads = numOptions;
    }

#if !defined(ENABLE_THREADS) && !defined(ENABLE_OPENMP) && !defined(ENABLE_TBB)
    if(nThreads != 1) {
        printf("Error: <nthreads> must be 1 (serial version)\n");
        exit(1);
    }
#endif
    

    // alloc spaces for the option data
    data = (OptionData*)malloc(numOptions*sizeof(OptionData));
    prices = (fptype*)malloc(numOptions*sizeof(fptype));
    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
		
		    // -----------PAPI-------------
		#ifdef SINGLE_PAPI
		if (loopnum % 2048 ==0)
		    waca_papi_read("Hello");  // Call when allocating memory
		#endif //SINGLE_PAPI
		    // -----------PAPI-------------
        rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &data[loopnum].s, &data[loopnum].strike, &data[loopnum].r, &data[loopnum].divq, &data[loopnum].v, &data[loopnum].t, &data[loopnum].OptionType, &data[loopnum].divs, &data[loopnum].DGrefval);
        if(rv != 9) {
          printf("ERROR: Unable to read from file `%s'.\n", inputFile);
          fclose(file);
          exit(1);
        }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }

#ifdef ENABLE_THREADS
    MAIN_INITENV(,8000000,nThreads);
#endif
    printf("Num of Options: %d\n", numOptions);
    printf("Num of Runs: %d\n", NUM_RUNS);

#define PAD 256
#define LINESIZE 64


    buffer = (fptype *) malloc(5 * numOptions * sizeof(fptype) + PAD);
    sptprice = (fptype *) (((unsigned long long)buffer + PAD) & ~(LINESIZE - 1));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;

    buffer2 = (int *) malloc(numOptions * sizeof(fptype) + PAD);
    otype = (int *) (((unsigned long long)buffer2 + PAD) & ~(LINESIZE - 1));

    for (i=0; i<numOptions; i++) {
        otype[i]      = (data[i].OptionType == 'P') ? 1 : 0;
        sptprice[i]   = data[i].s;
        strike[i]     = data[i].strike;
        rate[i]       = data[i].r;
        volatility[i] = data[i].v;    
        otime[i]      = data[i].t;
    }

    printf("Size of data: %ld\n", numOptions * (sizeof(OptionData) + sizeof(int)));
	
	
#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_begin();
#endif

#ifdef ENABLE_THREADS
#ifdef WIN32
    HANDLE *threads;
    int *nums;
    threads = (HANDLE *) malloc (nThreads * sizeof(HANDLE));
    nums = (int *) malloc (nThreads * sizeof(int));

	// WACA: Creation of threads: Imp

    for(i=0; i<nThreads; i++) {
        nums[i] = i;
        threads[i] = CreateThread(0, 0, bs_thread, &nums[i], 0, 0);
		
    }
    WaitForMultipleObjects(nThreads, threads, TRUE, INFINITE);
    free(threads);
    free(nums);
#else
    int *tids;
    tids = (int *) malloc (nThreads * sizeof(int));
    

    // -----------PAPI-------------
#ifdef SINGLE_PAPI
    waca_papi_read("Hello"); 	// PAPI read before thread creation as in Multithreaded instrumentation
    waca_papi_stop("Destroy");  // We need to remove the initialization of the PAPI single threaded before proceeding to multithreaded.
#endif //SINGLE_PAPI
    // -----------PAPI-------------

	//-----------------WACA: MULTI- PAPI Initialiation for multithreaded--------------------
	
	// Initialize PAPI library
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	
	if (retval != PAPI_VER_CURRENT) {
		fprintf(stderr, "PAPI library init error!\n");
		exit(1);
	}
	
	// PAPI thread initialization
	if (PAPI_thread_init(pthread_self) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__); 
	//-----------------WACA  MULTI- PAPI----------------------------------------------------------


    for(i=0; i<nThreads; i++) {
        tids[i]=i;
        CREATE_WITH_ARG(bs_thread, &tids[i]);
    }
    // ------ PAPI -------
    papi_init();

    // Start the timer
    if (clock_gettime(CLOCK_MONOTONIC , &begin)) {
      exit(EXIT_FAILURE);
    }

    // Start time in nanoseconds
    start = begin.tv_sec*NANOS + begin.tv_nsec;
    // ------ PAPI -------

    WAIT_FOR_END(nThreads);
    free(tids);


#endif //WIN32
#else //ENABLE_THREADS
#ifdef ENABLE_OPENMP
    {
        int tid=0;
        omp_set_num_threads(nThreads);
        bs_thread(&tid);
    }
#else //ENABLE_OPENMP
#ifdef ENABLE_TBB
    tbb::task_scheduler_init init(nThreads);

    int tid=0;
    bs_thread(&tid);
#else //ENABLE_TBB
    //serial version
    int tid=0;
    bs_thread(&tid);
#endif //ENABLE_TBB
#endif //ENABLE_OPENMP
#endif //ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_end();
#endif


    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }
    rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }
    for(i=0; i<numOptions; i++) {

	    // -----------PAPI-------------
	#ifdef SINGLE_PAPI
	if (i % 2048 ==0)   
		waca_papi_read("Hello"); 	// PAPI Read after the threads have been de-registered as in Multithreaded instrumentation
	#endif // SINGLE_PAPI
	    // -----------PAPI-------------

      rv = fprintf(file, "%.18f\n", prices[i]);
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(file);
        exit(1);
      }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }

#ifdef ERR_CHK
    printf("Num Errors: %d\n", numError);
#endif
    free(data);
    free(prices);

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_bench_end();
#endif

// ----- PAPI ---------
#ifdef SINGLE_PAPI
	//PAPI end
	waca_papi_end("Hello");
	waca_papi_stop("Destroy");
#endif //SINGLE_PAPI
// ----- PAPI ---------

    return 0;
}

