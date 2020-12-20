/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "semaphore.h"

#define PHIL_COUNT 10000
#define EAT_COUNT 100
#define BOWL_COUNT 2

typedef struct my_semaphore sem_b;

// Global Variables

sem_b forks[PHIL_COUNT];	   // Binary semaphore array for phil_run
sem_b sauce_bowls[BOWL_COUNT]; // Semaphores for sauce_bowls

// Helper functions
/**
 * @brief Send wait signal to semaphore at index of the semaphore array
 * 
 * @param sem_arr Array of semaphores
 * @param index Index to send wait() on
 */
void get_sem_at_index(sem_b *sem_arr, int index)
{
	if (wait_blocking(&sem_arr[index]) == -1)
	{
		printf("wait_blocking(): error\n");
		exit(1);
	}
}

/**
 * @brief Initialises a semphore array
 * 
 * @param sem_arr Array of semaphores
 * @param count Size of array
 */
void initialise_sem_array(sem_b *sem_arr, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (init_sem(&sem_arr[i], 1) == -1)
		{
			printf("init_sem(): error\n");
			exit(1);
		}
	}
}

/**
 * @brief The function which each philosopher runs 
 * 
 * @param number Philosopher index number
 * @return void* 
 */
void *phil_run(void *number)
{
	int current_phil = *((int *)number);
	int right_fork = (current_phil == PHIL_COUNT) ? 0 : current_phil;
	int curr_eat_count = 0;
	// Loop till philosopher hasn't eaten enough times
	do
	{
		//* Entry Section
		// Changing the order of getting the forks/chopsticks of half the philosophers prevents cyclic deadlock on getting forks
		if (current_phil % 2 != 0)
		{
			// Odd philosophers grab the right fork first and then the left one.
			get_sem_at_index(forks, right_fork);
			// printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);

			get_sem_at_index(forks, current_phil - 1);
			// printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);
		}
		else
		{
			// Even philosophers grab the left fork first and then the right one.
			get_sem_at_index(forks, current_phil - 1);
			// printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);

			get_sem_at_index(forks, right_fork);
			// printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);
		}

		//* Critical Section
		// Get both bowls one after the other to prevent deadlock
		for (int i = 0; i < BOWL_COUNT; i++)
		{
			get_sem_at_index(sauce_bowls, i);
			// print_count_blocking(&sauce_bowls[i]);
			// printf("Philosopher %d picked up bowl %d\n", current_phil, i + 1);
		}

		// Philosopher is eating!
		printf("Philosopher %d is eating with forks %d and %d!\n", current_phil, current_phil, right_fork + 1);

		//* Exit Section
		// Signal that bowls are available to take
		for (int i = BOWL_COUNT - 1; i >= 0; i--)
		{
			if (signal_blocking(&sauce_bowls[i]) == -1)
			{
				printf("signal_blocking(): error\n");
				exit(1);
			}
		}

		// Signal that the forks are now available to use!
		if (signal_blocking(&forks[current_phil - 1]) == -1)
		{
			printf("signal_blocking(): error\n");
			exit(1);
		}
		if (signal_blocking(&forks[right_fork]) == -1)
		{
			printf("signal_blocking(): error\n");
			exit(1);
		}

		//Increase the number of times the philosopher has eaten
		curr_eat_count++;

	} while (curr_eat_count != EAT_COUNT);

	// printf("Philosopher %d is done!\n", current_phil);
}

int main(void)
{
	pthread_t philosophers[PHIL_COUNT];

	// Initialise semaphores
	initialise_sem_array(forks, PHIL_COUNT);
	initialise_sem_array(sauce_bowls, BOWL_COUNT);

	int NUMBERS[PHIL_COUNT];
	for (int i = 0; i < PHIL_COUNT; i++)
	{
		NUMBERS[i] = i + 1;
	}

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

	return 0;
}