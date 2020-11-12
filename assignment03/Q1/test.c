/* Name: Rohan Jain
   Roll_Number: 2019095 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SYSCALL_NUMBER 440
#define LOOP_END 10000000000

/* Returns time taken to run a loop from 0 to LOOP_END */
double get_time()
{
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);
	for(long int i = 0; i < LOOP_END; i++){}
	gettimeofday(&end_time, NULL);
	long long diff_usec = end_time.tv_usec - start_time.tv_usec;
	if( diff_usec < 0){
		diff_usec += 1000000;
		end_time.tv_sec -= 1;
	}
	return (end_time.tv_sec - start_time.tv_sec + 0.000001*diff_usec);
	
}

int main(int argc, char **argv)
{
	pid_t pid, fork_pid;
	long long srt_val;
	long int sys_return;
	
	printf("Enter pid(-1 to send pid of this process): ");
	scanf("%d", &pid);
	printf("Enter soft realtime requirements: ");
	scanf("%lld", &srt_val);
		
	fork_pid = fork();
	if(fork_pid == -1){
		perror("fork: error");
		exit(1);
	}
	else if(fork_pid == 0){
		if(pid == -1){
			pid = getpid();
		}
		// Converting seconds to nanoseconds because syscall takes nanoseconds as param
		sys_return = syscall(SYSCALL_NUMBER, pid, srt_val * 1000000000);
		if (sys_return != 0)
		{
			perror("rtnice: error");
			return -1;
		}
		else{
			printf("Time taken with soft realtime requirements: %f\n", get_time());
		}
		exit(0);
	}
	else{	
		printf("Time taken normally: %f\n", get_time());
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("wait: error");
			exit(1);
		}
		exit(0);
	}
	return 0;
}
