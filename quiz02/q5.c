/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_SIZE 8 //8 byte Shared Memory
#define SHM_PERMS 0666
#define READER_COUNT 10
#define WRITER_COUNT 5
#define NUM_WRITES 1
#define NUM_READS 1

sem_t writing;		   //Binary semaphore for writer and reader
pthread_mutex_t mutex; // Mutex lock for reader_count
int reader_count = 0;  // Number of readers currently reading shm

void *reader(void *number)
{
	key_t shm_key;
	int shmid;
	// Generate key and connect to the shared memory
	if ((shm_key = ftok("q5.c", 'A')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((shmid = shmget(shm_key, SHM_SIZE, SHM_PERMS | IPC_CREAT)) == -1)
	{
		perror("shmget(): error");
		exit(1);
	}

	int num_reads = 0;
	do
	{
		//* Entry Section
		// Reader will acquire the mutex lock before updating the value of current number of readers
		if (pthread_mutex_lock(&mutex) != 0)
		{
			perror("pthread_mutex_lock(): error");
			exit(1);
		}
		reader_count++;

		// If this is the first reader send a signal down to semaphore so that writer doesn't write to shm
		if (reader_count == 1)
		{
			if (sem_wait(&writing) == -1)
			{
				perror("sem_wait(): error");
				exit(1);
			}
		}

		// Remove the mutex lock so other readers can update the value of curr_reader_count
		if (pthread_mutex_unlock(&mutex) != 0)
		{
			perror("pthread_mutex_unlock(): error");
			exit(1);
		}

		//* Critical Section
		// Attach to shm
		char *shm_ptr = shmat(shmid, (void *)0, 0);
		if (shm_ptr == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Read data
		char str[SHM_SIZE];
		strncpy(str, shm_ptr, SHM_SIZE);
		printf("Reader %d read %s\n", (*((int *)number)), str);

		// Detach from shm
		if (shmdt(shm_ptr) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//* Exit Section
		// Reader will acquire the mutex lock before updating the value of current number of readers
		if (pthread_mutex_lock(&mutex) != 0)
		{
			perror("pthread_mutex_lock(): error");
			exit(1);
		}
		reader_count--;

		// If this is the last reader then send a signal up to semaphore so that writer can write to shm
		if (reader_count == 0)
		{
			if (sem_post(&writing) == -1)
			{
				perror("sem_post(): error");
				exit(1);
			}
		}

		// Remove the mutex lock so other readers can update the value of reader_count
		if (pthread_mutex_unlock(&mutex) != 0)
		{
			perror("pthread_mutex_unlock(): error");
			exit(1);
		}
		num_reads++;
	} while (num_reads != NUM_READS);
}

void *writer(void *number)
{
	key_t shm_key;
	int shmid;
	// Generate key and connect to the shared memory
	if ((shm_key = ftok("q5.c", 'A')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((shmid = shmget(shm_key, SHM_SIZE, SHM_PERMS | IPC_CREAT)) == -1)
	{
		perror("shmget(): error");
		exit(1);
	}

	int num_writes = 0;
	do
	{
		//* Signal for down (Entry Section)
		if (sem_wait(&writing) == -1)
		{
			perror("sem_wait(): error");
			exit(1);
		}

		//* Critical Section
		// Attach to shm
		char *shm_ptr = shmat(shmid, (void *)0, 0);
		if (shm_ptr == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Write data
		char str[SHM_SIZE];
		sprintf(str, "%d", (*((int *)number)));
		printf("Writer %d wrote %s\n", (*((int *)number)), str);
		strncpy(shm_ptr, str, SHM_SIZE);

		// Detach from shm
		if (shmdt(shm_ptr) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//* Signal for Up (Exit Section)
		if (sem_post(&writing) == -1)
		{
			perror("sem_post(): error");
			exit(1);
		}
		num_writes++;
	} while (num_writes != NUM_WRITES);
}

int main(void)
{
	pthread_t readers[READER_COUNT], writers[WRITER_COUNT];
	// Open shared memory in main to destroy it after process ends
	key_t shm_key;
	int shmid;
	// reader writer count cannot be more than 10;
	int NUMBERS[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	// Initialise mutex and semaphores
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		perror("pthread_mutex_init(): error");
		exit(1);
	}
	if (sem_init(&writing, 0, 1) == -1)
	{
		perror("sem_init(): error");
		exit(1);
	}

	// Generate key and connect to the shared memory
	if ((shm_key = ftok("q5.c", 'A')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((shmid = shmget(shm_key, SHM_SIZE, SHM_PERMS | IPC_CREAT)) == -1)
	{
		perror("shmget(): error");
		exit(1);
	}
	// Attach to shm
	char *shm_ptr = shmat(shmid, (void *)0, 0);
	if (shm_ptr == (char *)(-1))
	{
		perror("shmat(): error");
		exit(1);
	}

	// Write data
	char str[SHM_SIZE] = "1";
	strncpy(shm_ptr, str, SHM_SIZE);

	// Detach from shm
	if (shmdt(shm_ptr) == -1)
	{
		perror("shmdt(): error");
		exit(1);
	}

	// Create threads
	for (int i = 0; i < WRITER_COUNT; i++)
	{
		if (pthread_create(&writers[i], NULL, (void *)writer, (void *)&NUMBERS[i]) != 0)
		{
			perror("pthread_create(): error");
			// Don't exit, let other threads run.
		}
	}

	for (int i = 0; i < READER_COUNT; i++)
	{
		if (pthread_create(&readers[i], NULL, (void *)reader, (void *)&NUMBERS[i]) != 0)
		{
			perror("pthread_create(): error");
			// Don't exit, let other threads run.
		}
	}

	// Join threads
	for (int i = 0; i < WRITER_COUNT; i++)
	{
		if (pthread_join(writers[i], NULL) != 0)
		{
			perror("pthread_join(): error");
			// Don't exit, Let the other threads join
		}
	}

	for (int i = 0; i < READER_COUNT; i++)
	{
		if (pthread_join(readers[i], NULL) != 0)
		{
			perror("pthread_join(): error");
			// Don't exit, Let the other threads join
		}
	}

	// Cleanup: Destroy shared memory, mutex and semaphores
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
	{
		perror("shmctl(): error");
		exit(1);
	}
	if (pthread_mutex_destroy(&mutex) != 0)
	{
		perror("pthread_mutex_destroy(): error");
		exit(1);
	}
	if (sem_destroy(&writing) == -1)
	{
		perror("sem_destroy(): error");
		exit(1);
	}

	return 0;
}