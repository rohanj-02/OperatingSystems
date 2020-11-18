/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define PHIL_COUNT 5
#define EAT_COUNT 1

sem_t forks[PHIL_COUNT]; //Binary semaphore array for phil_run

void *phil_run(void *number)
{
	int current_phil = *((int *)number);
	int right_fork = (current_phil == PHIL_COUNT) ? 0 : current_phil;
	int curr_eat_count = 0;

	// Loop till philosopher hasn't eaten enough times
	do
	{
		//* Entry Section
		// Changing the order of getting the forks/ chopsticks of half the philosophers prevents deadlock
		if (current_phil % 2 != 0)
		{
			// Odd philosophers grab the right fork first and then the left one.
			if (sem_wait(&forks[right_fork]) == -1)
			{
				perror("sem_wait(): error");
				exit(1);
			}

			printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);

			if (sem_wait(&forks[current_phil - 1]) == -1)
			{
				perror("sem_wait(): error");
				exit(1);
			}

			printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);
		}
		else
		{
			// Even philosophers grab the left fork first and then the right one.
			if (sem_wait(&forks[current_phil - 1]) == -1)
			{
				perror("sem_wait(): error");
				exit(1);
			}

			printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);

			if (sem_wait(&forks[right_fork]) == -1)
			{
				perror("sem_wait(): error");
				exit(1);
			}
			printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);
		}

		//* Critical Section
		printf("Philosopher %d is eating!\n", current_phil);

		//* Exit Section
		// Signal that the forks are now available to use!
		if (sem_post(&forks[current_phil - 1]) == -1)
		{
			perror("sem_post(): error");
			exit(1);
		}
		if (sem_post(&forks[right_fork]) == -1)
		{
			perror("sem_post(): error");
			exit(1);
		}

		//Increase the number of times the philosopher has eaten
		curr_eat_count++;

	} while (curr_eat_count != EAT_COUNT);

	printf("Philosopher %d is done!\n", current_phil);
}

int main(void)
{
	pthread_t philosophers[PHIL_COUNT];

	// Initialise semaphores
	for (int i = 0; i < PHIL_COUNT; i++)
	{
		if (sem_init(&forks[i], 0, 1) == -1)
		{
			perror("sem_init(): error");
			exit(1);
		}
	}

	// philosopher count cannot be more than 10;
	int NUMBERS[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	// Create threads and then join them
	for (int i = 0; i < PHIL_COUNT; i++)
	{
		if (pthread_create(&philosophers[i], NULL, (void *)phil_run, (void *)&NUMBERS[i]) != 0)
		{
			perror("pthread_create(): error");
			// Don't exit, let other threads run.
		}
	}

	for (int i = 0; i < PHIL_COUNT; i++)
	{
		if (pthread_join(philosophers[i], NULL) != 0)
		{
			perror("pthread_join(): error");
			// Don't exit, Let the other threads join
		}
	}

	// Cleanup: Destroy semaphores
	for (int i = 0; i < PHIL_COUNT; i++)
	{
		if (sem_destroy(&forks[i]) == -1)
		{
			perror("sem_destroy(): error");
			exit(1);
		}
	}

	return 0;
}