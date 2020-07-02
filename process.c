#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include "process.h"

long long int cur_time(){
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return t.tv_sec * 1000000000ll + t.tv_nsec;
}

void unit_time()				
{						
	volatile unsigned long i;		
	for (i = 0; i < 1000000UL; i++);	
}	
void set_CPU(int pid, int affinity)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(affinity, &mask);
	if(sched_setaffinity(pid, sizeof(mask), &mask) < 0)
	{
		perror("sched_setaffinity error");
		exit(EXIT_FAILURE);
	}
	return;
}
int create_process(process p)
{
	int pid = fork();
	//p.start_time = cur_time();
	if(pid < 0)
	{
		perror("fork error");
		return -1;
	}
	else if(pid == 0)
	{
		for(int i = 0; i < p.burst_t;i++)
		{

			unit_time();
		}
		
		exit(0);
	}
	set_CPU(pid, 1);
	return pid;
}
void set_scheduler(pid_t pid,int priority)
{
    struct sched_param param;
    param.sched_priority = priority;
    if(sched_setscheduler(pid,SCHED_FIFO,&param)!=0)
    {
        perror("sched_setscheduler error");
        exit(EXIT_FAILURE);
    }
}
