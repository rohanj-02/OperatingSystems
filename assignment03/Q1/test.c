/* Name: Rohan Jain
   Roll_Number: 2019095 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SYSCALL_NUMBER 440
#define LOOP_END 10000000000

double get_time()
{
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);
	for(long int i = 0; i < LOOP_END; i++){}
	gettimeofday(&end_time, NULL);
	double timeDiff = end_time.tv_sec - start_time.tv_sec + 0.000001*(end_time.tv_usec - start_time.tv_usec);
	return timeDiff;
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
		perror("Fork error");
		exit(1);
	}
	else if(fork_pid == 0){
		sys_return = syscall(SYSCALL_NUMBER, getpid(), srt_val * 1000000000);
		if (sys_return != 0)
		{
			perror("Error: rtnice");
			return -1;
		}
		else{
			printf("Realtime time: %f\n", get_time());
		}
		exit(0);
	}
	else{	
		printf("CFS time: %f\n", get_time());
		exit(0);
	}
/*	
	if (pid == -1)
	{
		sys_return = syscall(SYSCALL_NUMBER, getpid(), srt_val);
	}
	else
	{
		sys_return = syscall(SYSCALL_NUMBER, pid, srt_val);
	}
	
	
	if (sys_return != 0)
	{
		perror("Error: rtnice");
		return -1;
	}
*/	
//	printf("Run dmesg or see the file in your current directory to check the output.\n");
	return 0;
}
