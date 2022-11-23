/*-------------------------------------------------------------------------
 * cpu_affinity.c
 *		POSTGRES pluggable CPU Affinity
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  src/backend/postmaster/cpu_affinity.c
 *
 *-------------------------------------------------------------------------
 */

#include "postmaster/cpu_affinity.h"

char *
CPUAffinityGetTextDatum(int pid)
{
	cpu_set_t cpu_set;
	static char result[MAX_RST_LEN];	
	Onion *oni;
	char *mem;
	int cpu_setsize = 0;
	int *cpu_mark, *p, *q;
	int i = 0, j = 0, k = 0, len = 0;

	memset(result,0,MAX_RST_LEN);
	cpu_setsize = sysconf(_SC_NPROCESSORS_CONF);	
	cpu_mark = palloc(cpu_setsize * sizeof(int));
	mem = palloc(cpu_setsize * sizeof(char));
	oni = palloc(cpu_setsize * sizeof(Onion));
	
	CPU_ZERO(&cpu_set);

	if (sched_getaffinity((pid_t)pid, sizeof(cpu_set_t), &cpu_set))
		ereport(ERROR,(errmsg("sched_getaffinity failed")));
	
	for (i = 0; i < cpu_setsize; i++)
		CPU_ISSET(i, &cpu_set) ? cpu_mark[j++] = i: 1;
		
	if (p = cpu_mark , j > 0)
	{
		if (q = p + 1, j > 1 )		
		{	
			while(k < j)
			{
				if (q - p == *q - *p )
				{
					oni[k].a = *p;
					oni[k].b = *q;

					if (q - cpu_mark + 1 < j)
						q++;
					else
					{
						oni[k++].b = *q;
						break;
					}
				}
				else if (q - p < *q - *p)
				{
					if (q > p + 1)
					{
						p = q,k++;
						if (q - cpu_mark + 1 < j)
							q++;
					}
					else
					{
						oni[k].a = *p;
						oni[k++].b = *p;
						p = q;
						if (q - cpu_mark + 1 < j)
							q++;
					}
				}
				else if (q == p)
				{
					oni[k].a = *p;
					oni[k].b = *q;
					break;
				}
				else
				{
					oni[k].a = *p;
					oni[k++].b = *p;
					p++;
				}
			}	// while	

			for (i = 0; i < k; i++)
			{
				if (oni[i].a != oni[i].b)
					sprintf(mem,"%d-%d,",oni[i].a,oni[i].b);
				else
					sprintf(mem,"%d,",oni[i].a);

				strcat(result,mem);
				len = len + strlen(mem);
			}
			result[len - 1] = 0;
		}
		else	// j = 1
		{
			sprintf(mem,"%d",*p);
			strcat(result,mem);
			return result;
		}
	}

	ereport(DEBUG1, (errmsg("result:%s", result)));
	
	return result;
}
