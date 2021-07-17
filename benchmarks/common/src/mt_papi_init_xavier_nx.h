	// Store counter data
	long_long values[35];

	// Thread specific variables
	struct timespec thread_begin;
	long long thread_start;

	int thread_papi_skip_counter = 0;
	
	// Register thread
	retval = PAPI_register_thread(  );
	if ( retval != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
	// Create EventSet for counters
	if (PAPI_create_eventset(&EventSet) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
	// Instructions
	if ( PAPI_event_name_to_code("PAPI_TOT_INS", &EventCode) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Add instruction PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// CPU Cycles
	if ( PAPI_event_name_to_code( "PAPI_TOT_CYC", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Add CPU Cycles PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Branch Miss Prediction
	if ( PAPI_event_name_to_code( "PAPI_BR_MSP", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Add Branch Miss PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Level 2 cache misses
	if ( PAPI_event_name_to_code( "PAPI_L2_DCM", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Add Level 2 cache misses PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Data Memory Access
	if ( PAPI_event_name_to_code( "PAPI_L2_DCA", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Add Data Memory Access PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);

	// Noncache external memory request
	if ( PAPI_event_name_to_code( "PAPI_L1_ICM", &EventCode ) != PAPI_OK )
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
	// Add Noncache external memory request PAPI code to EventSet
	if (PAPI_add_event(EventSet, EventCode) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__);
	
	// Start PAPI calls.
	if (PAPI_start(EventSet) != PAPI_OK)
		printf ("%s:%d\t ERROR\n", __FILE__, __LINE__); 

        // Start the timer
        if (clock_gettime(CLOCK_MONOTONIC , &thread_begin)) {
            exit(EXIT_FAILURE);
        }

        // Start time in nanoseconds
        thread_start = thread_begin.tv_sec*NANOS + thread_begin.tv_nsec;

