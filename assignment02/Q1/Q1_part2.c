/* Name: Rohan Jain
   Roll_Number: 2019095 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int number = 10;

void *decrement(void *argument)
{
	printf("Child Thread created!\n");
	while (number > -90)
	{
		number -= 1;
		printf("Child Thread: %d\n", number);
	}
	printf("Child thread Terminating!\n");
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t child;
	if (pthread_create(&child, NULL, decrement, NULL))
	{
		perror("pthread_create: error");
		exit(1);
	}
	printf("Parent Thread running!\n");
	while (number < 100)
	{
		number += 1;
		printf("Parent Thread: %d\n", number);
	}
	pthread_join(child, NULL);
	printf("Parent Thread Terminating!\n");
	pthread_exit(NULL);
	return 0;
}