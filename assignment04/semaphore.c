/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define PHIL_COUNT 10000
#define EAT_COUNT 10
#define BOWL_COUNT 2

typedef struct my_blocking_semaphore sem_b;

// TODO Error handling

// Semaphore functions
struct my_blocking_semaphore
{
	pthread_mutex_t lock;
	pthread_cond_t wait;
	int value;
	int num_waiters;
};

int init_sem_blocking(sem_b *my_sem, int initial_value)
{
	my_sem->value = initial_value;
	if (pthread_mutex_init(&(my_sem->lock), NULL) != 0)
	{
		perror("pthread_mutex_init(): error");
		return -1;
	}
	if (pthread_cond_init(&(my_sem->wait), NULL) != 0)
	{
		perror("pthread_cond_init(): error");
		return -1;
	}
	my_sem->num_waiters = 0;
	return 0;
}

int signal_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	s->value++;

	if (s->value <= 0)
	{
		if (pthread_cond_signal(&(s->wait)) != 0)
		{
			perror("pthread_cond_signal(): error");
			return -1;
		}
	}

	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
	return 0;
}

int wait_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	s->value--;

	while (s->value < 0)
	{
		if (s->num_waiters >= (-1 * s->value))
		{
			break;
		}
		else
		{
			s->num_waiters++;
			if (pthread_cond_wait(&(s->wait), &(s->lock)) != 0)
			{
				perror("pthread_cond_wait(): error");
				return -1;
			}

			s->num_waiters--;
		}
	}
	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}

	return 0;
}

void print_value_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	printf("Value of semaphore: %d\n", s->value);
	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
}

// Global Variables

sem_b forks[PHIL_COUNT];	   // Binary semaphore array for phil_run
sem_b sauce_bowls[BOWL_COUNT]; // Semaphores for sauce_bowls

// Helper functions

void get_sem_at_index(sem_b *sem_arr, int index)
{
	if (wait_blocking(&sem_arr[index]) == -1)
	{
		printf("wait_blocking(): error");
		exit(1);
	}
}

void initialise_sem_array(sem_b *sem_arr, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (init_sem_blocking(&sem_arr[i], 1) == -1)
		{
			printf("init_sem_blocking(): error");
			exit(1);
		}
	}
}

// Philosopher thread
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
			get_sem_at_index(forks, right_fork);
			printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);

			get_sem_at_index(forks, current_phil - 1);
			printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);
		}
		else
		{
			// Even philosophers grab the left fork first and then the right one.
			get_sem_at_index(forks, current_phil - 1);
			printf("Philosopher %d picked up fork %d\n", current_phil, current_phil);

			get_sem_at_index(forks, right_fork);
			printf("Philosopher %d picked up fork %d\n", current_phil, right_fork + 1);
		}

		//* Critical Section
		// Get both bowls to eat
		for (int i = 0; i < BOWL_COUNT; i++)
		{
			get_sem_at_index(sauce_bowls, i);
			printf("Philosopher %d picked up bowl %d\n", current_phil, i + 1);
		}

		// Philosopher is eating!
		printf("Philosopher %d is eating!\n", current_phil);

		//* Exit Section
		// Signal that bowls are available to take
		for (int i = BOWL_COUNT - 1; i >= 0; i--)
		{
			if (signal_blocking(&sauce_bowls[i]) == -1)
			{
				printf("signal_blocking(): error");
				exit(1);
			}
		}
		// Signal that the forks are now available to use!
		if (signal_blocking(&forks[current_phil - 1]) == -1)
		{
			printf("signal_blocking(): error");
			exit(1);
		}
		if (signal_blocking(&forks[right_fork]) == -1)
		{
			printf("signal_blocking(): error");
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