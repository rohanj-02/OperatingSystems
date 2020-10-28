/* Name: Rohan Jain
   Roll_Number: 2019095 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int number = 10;

void changeGlobalVariable(int size, int increment, char *prefix)
{
	for (int i = 0; i < size; i++)
	{
		number += increment;
		printf("%s: %d\n", prefix, number);
	}
	return;
}

void incrementUptoMax(int max, char *prefix)
{
	while (number < max)
	{
		number += 1;
		printf("%s: %d\n", prefix, number);
	}
	return;
}

void decrementUptoMin(int min, char *prefix)
{
	while (number > min)
	{
		number -= 1;
		printf("%s: %d\n", prefix, number);
	}
	return;
}

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
	{
		perror("fork: error");
		exit(1);
	}
	else if (pid == 0)
	{
		printf("Child process running! PID is: %ld\n", (long)getpid());
		decrementUptoMin(-90, "Child Process");
		printf("Terminating Child Process! \n");
		exit(0);
	}
	else
	{
		printf("Parent Process running! PID is: %ld\n", (long)getpid());
		incrementUptoMax(100, "Parent Process");
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid: error");
			exit(1);
		}
		printf("Terminating Parent Process! \n");
		exit(0);
	}
	return 0;
}