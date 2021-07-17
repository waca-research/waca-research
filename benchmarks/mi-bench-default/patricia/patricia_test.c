/*
 * patricia_test.c
 *
 * Patricia trie test code.
 *
 * This code is an example of how to use the Patricia trie library for
 * doing longest-prefix matching.  We begin by adding a default
 * route/default node as the head of the Patricia trie.  This will become
 * an initialization functin (pat_init) in the future.  We then read in a
 * set of IPv4 addresses and network masks from "pat_test.txt" and insert
 * them into the Patricia trie.  I haven't _yet_ added example of searching
 * and removing nodes.
 *
 * Compiling the library:
 *     gcc -g -Wall -c patricia.c
 *     ar -r libpatricia.a patricia.o
 *     ranlib libpatricia.a
 *
 * Compiling the test code (or any other file using libpatricia):
 *     gcc -g -Wall -I. -L. -o ptest patricia_test.c -lpatricia
 *
 * Matthew Smart <mcsmart@eecs.umich.edu>
 *
 * Copyright (c) 2000
 * The Regents of the University of Michigan
 * All rights reserved
 *
 * $Id: patricia_test.c,v 1.1.1.1 2000/11/06 19:53:17 mguthaus Exp $
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <rpc/rpc.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "patricia.h"

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
int skip_papi_cntr_threshold = 2048;
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


struct MyNode {
	int foo;
	double bar;
};

int
main(int argc, char **argv)
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



	struct ptree *phead;
	struct ptree *p,*pfind;
	struct ptree_mask *pm;
	FILE *fp;
	char line[128];
	char addr_str[16];
	struct in_addr addr;
	unsigned long mask=0xffffffff;
	float time;

	if (argc<2) {
		printf("Usage: %s <TCP stream>\n", argv[0]);
		exit(-1);
	}
	/*
	 * Open file of IP addresses and masks.
	 * Each line looks like:
	 *    10.0.3.4 0xffff0000
	 */
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("File %s doesn't seem to exist\n",argv[1]);
		exit(0);
	}

	/*
	 * Initialize the Patricia trie by doing the following:
	 *   1. Assign the head pointer a default route/default node
	 *   2. Give it an address of 0.0.0.0 and a mask of 0x00000000
	 *      (matches everything)
	 *   3. Set the bit position (p_b) to 0.
	 *   4. Set the number of masks to 1 (the default one).
	 *   5. Point the head's 'left' and 'right' pointers to itself.
	 * NOTE: This should go into an intialization function.
	 */
	phead = (struct ptree *)malloc(sizeof(struct ptree));
	if (!phead) {
		perror("Allocating p-trie node");
		exit(0);
	}
	bzero(phead, sizeof(*phead));
	phead->p_m = (struct ptree_mask *)malloc(
			sizeof(struct ptree_mask));
	if (!phead->p_m) {
		perror("Allocating p-trie mask data");
		exit(0);
	}
	bzero(phead->p_m, sizeof(*phead->p_m));
	pm = phead->p_m;
	pm->pm_data = (struct MyNode *)malloc(sizeof(struct MyNode));
	if (!pm->pm_data) {
		perror("Allocating p-trie mask's node data");
		exit(0);
	}
	bzero(pm->pm_data, sizeof(*pm->pm_data));
	/*******
	 *
	 * Fill in default route/default node data here.
	 *
	 *******/
	phead->p_mlen = 1;
	phead->p_left = phead->p_right = phead;


	/*
	 * The main loop to insert nodes.
	 */
	while (fgets(line, 128, fp)) {
		/*
		 * Read in each IP address and mask and convert them to
		 * more usable formats.
		 */

		sscanf(line, "%f %d", &time, (unsigned int *)&addr);
		//inet_aton(addr_str, &addr);

		/*
		 * Create a Patricia trie node to insert.
		 */
		p = (struct ptree *)malloc(sizeof(struct ptree));
		if (!p) {
			perror("Allocating p-trie node");
			exit(0);
		}
		bzero(p, sizeof(*p));

		/*
		 * Allocate the mask data.
		 */
		p->p_m = (struct ptree_mask *)malloc(
				sizeof(struct ptree_mask));
		if (!p->p_m) {
			perror("Allocating p-trie mask data");
			exit(0);
		}
		bzero(p->p_m, sizeof(*p->p_m));

		/*
		 * Allocate the data for this node.
		 * Replace 'struct MyNode' with whatever you'd like.
		 */
		pm = p->p_m;
		pm->pm_data = (struct MyNode *)malloc(sizeof(struct MyNode));
		if (!pm->pm_data) {
			perror("Allocating p-trie mask's node data");
			exit(0);
		}
		bzero(pm->pm_data, sizeof(*pm->pm_data));

		/*
		 * Assign a value to the IP address and mask field for this
		 * node.
		 */
		p->p_key = addr.s_addr;		/* Network-byte order */
		p->p_m->pm_mask = htonl(mask);

		pfind=pat_search(addr.s_addr,phead);
		//printf("%08x %08x %08x\n",p->p_key, addr.s_addr, p->p_m->pm_mask);
		//if(pfind->p_key==(addr.s_addr&pfind->p_m->pm_mask))
		if(pfind->p_key==addr.s_addr)
		{
			printf("%f %08x: ", time, addr.s_addr);
			printf("Found.\n");
		}
		else
		{
			/*
		 	* Insert the node.
		 	* Returns the node it inserted on success, 0 on failure.
		 	*/
			//printf("%08x: ", addr.s_addr);
			//printf("Inserted.\n");

			p = pat_insert(p, phead);
		}
		if (!p) {
			fprintf(stderr, "Failed on pat_insert\n");
			exit(0);
		}
	}

	waca_papi_end("Hello");     //-----------WACA: Read PAPI at end--------------------

	exit(1);
}
