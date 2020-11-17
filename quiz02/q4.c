#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sem.h>

#define SHM_SIZE 2048
#define SHM_PERM 0644
#define MAX_RETRIES 10

union semun
{
	int val;			   // for SETVAL
	struct semid_ds *buf;  //for IPC_STAT, IPC_SET
	unsigned short *array; // for GETALL, SETALL
};

int initsem(key_t key, int nsems) /* key from ftok() */
{
	int i;
	union semun arg;
	struct semid_ds buf;
	struct sembuf sb;
	int semid;
	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);
	if (semid >= 0)
	{ /* we got it first */
		sb.sem_op = 1;
		sb.sem_flg = 0;
		arg.val = 1;
		printf("press return\n");
		getchar();
		for (sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++)
		{
			/* do a semop() to "free" the semaphores. */
			/* this sets the sem_otime field, as needed below. */
			if (semop(semid, &sb, 1) == -1)
			{
				int e = errno;
				semctl(semid, 0, IPC_RMID); /* clean up */
				errno = e;
				return -1; /* error, check errno */
			}
		}
	}
	else if (errno == EEXIST)
	{ /* someone else got it first */
		int ready = 0;
		semid = semget(key, nsems, 0); /* get the id */
		if (semid < 0)
			return semid; /* error, check errno */
		/* wait for other process to initialize the semaphore: */
		arg.buf = &buf;
		for (i = 0; i < MAX_RETRIES && !ready; i++)
		{
			semctl(semid, nsems - 1, IPC_STAT, arg);
			if (arg.buf->sem_otime != 0)
			{
				ready = 1;
			}
			else
			{
				sleep(1);
			}
		}
		if (!ready)
		{
			errno = ETIME;
			return -1;
		}
	}
	else
	{
		return semid; /* error, check errno */
	}
	return semid;
}

int main(int argc, char *argv[])
{
	// shm is shared memory segment everywhere
	pid_t pid;
	pid = fork();

	if (pid == -1)
	{
		perror("fork(): error");
		exit(1);
	}
	else if (pid == 0)
	{
		// Child
		key_t shm_text_key, shm_num_key;
		int shmid_text, shmid_num;
		char *shmptr;
		// Generate key to connect to shm
		if ((shm_text_key = ftok("q4.c", 'A')) == -1)
		{
			perror("ftok(): error");
			exit(1);
		}

		// Create the shared mem segment if not created
		if ((shmid_text = shmget(shm_text_key, SHM_SIZE, SHM_PERM | IPC_CREAT)) == -1)
		{
			perror("shmget(): error");
			exit(1);
		}

		// Get pointer to shm at 0th position
		shmptr = shmat(shmid_text, (void *)0, 0);
		if (shmptr == (char *)(-1))
		{
			perror("shmat");
			exit(1);
		}
	}
	else
	{
		// Parent
		key_t key;
		int shmid_text, shmid_num;
		char *shmptr;

		// Generate key to connect to shm
		if ((key = ftok("q4.c", 'A')) == -1)
		{
			perror("ftok(): error");
			exit(1);
		}

		// Create the shared mem segment if not created
		if ((shmid_text = shmget(key, SHM_SIZE, SHM_PERM | IPC_CREAT)) == -1)
		{
			perror("shmget(): error");
			exit(1);
		}

		// Get pointer to shm at 0th position
		shmptr = shmat(shmid_text, (void *)0, 0);
		if (shmptr == (char *)(-1))
		{
			perror("shmat");
			exit(1);
		}
	}

	return 0;
}
