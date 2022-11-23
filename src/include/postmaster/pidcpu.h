/* ----------
 *  pidcpu.h
 *
 *  Definitions for the PostgreSQL statistics collector daemon.
 *
 *  Copyright (c) 2001-2020, PostgreSQL Global Development Group
 *
 *  src/include/postmaster/pidcpu.h
 * ----------
 */
#ifndef PIDCPU_H
#define PIDCPU_H

#include "dirent.h"
#include "fcntl.h"
#include "limits.h"
#include "postgres.h"
#include "sys/stat.h"
#include "unistd.h"

#define isdigit_string(_s)	isdigit_strend(_s, NULL)
#define _DEVICES_SYS_NODE_ "/sys/devices/system/node"
#define _SSCANF_PARAMS_  																		\
	"%c "																						\
	"%d %d %d %d %d "																			\
	"%lu %lu %lu %lu %lu "  																	\
	"%Lu %Lu %Lu %Lu "   																		\
	"%ld %ld "																					\
	"%d "																						\
	"%ld "																						\
	"%Lu "																						\
	"%llu "																						\
	"%ld "																						\
	"%llu " "%llu " "%llu " "%llu " "%llu " "%llu "												\
	"%*s %*s %*s %*s "																			\
	"%llu" "%*u %*u "																			\
	"%d %d "																					\
	"%llu %llu",																				\
	&P->state,																					\
	&P->ppid, &P->pgrp, &P->session, &P->tty, &P->tpgid,										\
	&P->flags, &P->min_flt, &P->cmin_flt, &P->maj_flt, &P->cmaj_flt,							\
	&P->utime, &P->stime, &P->cutime, &P->cstime,												\
	&P->priority, &P->nice,																		\
	&P->nlwp,																					\
	&P->alarm,																					\
	&P->start_time,																				\
	&P->vsize,																					\
	&P->rss,																					\
	&P->rss_rlim, &P->start_code, &P->end_code, &P->start_stack, &P->kstk_esp, &P->kstk_eip,	\
	&P->wchan,																					\
	&P->exit_signal, &P->processor,																\
	&P->rtprio, &P->sched												


typedef struct 
{
    int						ppid;       
    char 					cmd[32];
    char					state;     
    unsigned long long 		utime;
    unsigned long long		stime;      
    unsigned long long		cutime;     
    unsigned long long		cstime;     
    unsigned long long		start_time; 
    unsigned long long		start_code; 
    unsigned long long		end_code;   
    unsigned long long		start_stack;    
    unsigned long long		kstk_esp;   
    unsigned long long		kstk_eip;   
    unsigned long long		wchan;      
    long					priority;   
    long					nice;       
    long					rss;        
    long					alarm;      
    unsigned long long		rtprio;     
    unsigned long long		sched;      
    unsigned long long		vsize;      
    unsigned long long		rss_rlim;   
    unsigned long   		flags;      
    unsigned long   		min_flt;    
    unsigned long   		maj_flt;    
    unsigned long   		cmin_flt;   
    unsigned long   		cmaj_flt;   
    int 					pgrp;
    int 					session;
    int 					nlwp;
    int 					tty;
    int 					tpgid;
    int 					exit_signal;
    int 					processor; 
	char					errmsg[64];
} proc_t;

struct lscpu_cxt {
    int maxcpus;        /* size in bits of kernel cpu mask */
    int cflag;
    size_t nnodes;      /* number of NUMA modes */
    int *cpuarr;        /* arr for cpu nodes */
};

int GetPidProcStat(pid_t pid, proc_t *p);
int file2str(const char *dir, const char * what, char * ret, int cap);
int isdigit_strend(const char *str, const char **end);
int lscpu_read_numas(struct lscpu_cxt *cxt);
void lscpu_cxt_fini(struct lscpu_cxt *cxt);
void lscpu_cxt_init(struct lscpu_cxt *cxt);
void parse_cpu_list(char* cpu_list, struct lscpu_cxt* cpu_set);
void stat2proc(char *S, proc_t *__restrict__ P, int is_proc);
#endif
