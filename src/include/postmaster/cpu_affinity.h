/*--------------------------------------------------------------------
 * cpu_affinity.h
 *		POSTGRES pluggable CPU Affinity
 *
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *		src/include/postmaster/cpu_affinity.h
 *--------------------------------------------------------------------
 */
#ifndef CPU_AFFINITY_H
#define CPU_AFFINITY_H

/*---------------------------------------------------------------------
 * External module API.
 *---------------------------------------------------------------------
 */

#include <sched.h>
#include <unistd.h>
#include "postgres.h"
#include "postmaster/postmaster.h"

#define MAX_RST_LEN  4096

typedef struct
{
	int a;
	int b;
} Onion;


extern char * CPUAffinityGetTextDatum(int pid);

#endif							/* CPU_AFFINITY_H */
