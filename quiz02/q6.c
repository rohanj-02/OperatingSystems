#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define PHIL_COUNT 5

sem_t forks[PHIL_COUNT]; //Binary semaphore for phil_run and reader
// pthread_mutex_t mutex;	 // Mutex lock for reader_count

void *phil_run(void *number)
{
	//* Entry Section
	// Grab the forks!
	int current_phil = *((int *)number);
	// Changing the order of getting the forks/ chopsticks of one philosopher prevents deadlock

	int x = (current_phil == PHIL_COUNT) ? 0 : current_phil;
	if (current_phil == 1)
	{
		if (sem_wait(&forks[x]) == -1)
		{
			perror("sem_wait(): error");
			exit(1);
		}
		if (sem_wait(&forks[current_phil - 1]) == -1)
		{
			perror("sem_wait(): error");
			exit(1);
		}
	}
	else
	{
		if (sem_wait(&forks[current_phil - 1]) == -1)
		{
			perror("sem_wait(): error");
			exit(1);
		}
		if (sem_wait(&forks[x]) == -1)
		{
			perror("sem_wait(): error");
			exit(1);
		}
	}

	//* Critical Section
	fprintf(stderr, "Philosopher %d picked up forks %d and %d\n", current_phil, current_phil, x + 1);

	//* Exit Section
	if (sem_post(&forks[current_phil - 1]) == -1)
	{
		perror("sem_post(): error");
		exit(1);
	}
	if (sem_post(&forks[x]) == -1)
	{
		perror("sem_post(): error");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	pthread_t philosophers[PHIL_COUNT];

	// Initialise mutex and semaphores
	for (int i = 0; i < PHIL_COUNT; i++)
	{
		if (sem_init(&forks[i], 0, 1) == -1)
		{
			perror("sem_init(): error");
			exit(1);
		}
	}

	int NUMBERS[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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