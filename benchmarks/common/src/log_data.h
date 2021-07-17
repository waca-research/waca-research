#ifndef _LOG_DATA_FILE
#define _LOG_DATA_FILE

// Function declaration      
void log_data(long long *values, int tid, long long microseconds, long long timestamp);
void write_data(long long *values,long long microseconds);
int waca_papi_read (const char *format, ...);
int waca_papi_end (const char *format, ...); 
int mt_waca_papi_read(int EventSet, long long values[],int tid, long long *thread_start, int *thread_papi_skip_counter);
void papi_init();

#endif
