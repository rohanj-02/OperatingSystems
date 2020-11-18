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

sem_t writing;		   //Binary semaphore for writer and reader
pthread_mutex_t mutex; // Mutex lock for reader_count
int reader_count = 0;  // Number of readers currently reading shm
key_t shm_key;		   // For shared memory
int shmid;

//TODO See how to see that only readers don't hog up and writers dont starve(Do we need to check write after write or can we do without that?)
// If yes then find NUMBERS way to implement the producer consumer buffer thing here
//TODO Delete shared mem in main()
void *reader(void *number)
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
}

void *writer(void *number)
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
	sprintf(str, "writer%d", (*((int *)number)));
	printf("Writer wrote %s\n", str);
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
}

int main(int argc, char *argv[])
{
	pthread_t readers[READER_COUNT], writers[WRITER_COUNT];

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
	if ((shm_key = ftok("q5.c", 'NUMBERS')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((shmid = shmget(shm_key, SHM_SIZE, SHM_PERMS | IPC_CREAT)) == -1)
	{
		perror("shmget(): error");
		exit(1);
	}

	// reader writer count cannot be more than 10;
	int NUMBERS[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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

	for (int i = 0; i < READER_COUNT; i++)
	{
		if (pthread_join(readers[i], NULL) != 0)
		{
			perror("pthread_join(): error");
			// Don't exit, Let the other threads join
		}
	}

	for (int i = 0; i < WRITER_COUNT; i++)
	{
		if (pthread_join(writers[i], NULL) != 0)
		{
			perror("pthread_join(): error");
			// Don't exit, Let the other threads join
		}
	}

	// Cleanup: Destroy mutex and semaphores
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