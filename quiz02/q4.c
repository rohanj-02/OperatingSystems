#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sem.h>

#define SHM_SIZE 2048
#define SHM_PERM 0666
#define SEM_PERMS 0666
#define NUM_SEMS 3
#define MUTEX 0
#define NUM_SEG_LOCK 1
#define TEXT_SEG_LOCK 2
#define NUM_MSG 3724
#define TEXT_MSG "Rohan"

union semun
{
	int val;			   // for SETVAL
	struct semid_ds *buf;  // for IPC_STAT, IPC_SET
	unsigned short *array; // for GETALL, SETALL
};

int main(void)
{
	// shm is shared memory segment everywhere
	pid_t pid;
	key_t sem_key;
	int semid;

	// Initialise a named semaphore and then connect to it in each process, child and parent.
	if ((sem_key = ftok("q4.c", 'A')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((semid = semget(sem_key, NUM_SEMS, SEM_PERMS | IPC_CREAT)) == -1)
	{
		perror("semget(): error");
		exit(1);
	}

	// Set value of mutex to 1 and read locks to 0 as they will be locked till the writer writes to the shared memory
	ushort init_val[] = {1, 0, 0};
	union semun arg;
	arg.array = init_val;
	if (semctl(semid, 0, SETALL, arg.array) == -1)
	{
		perror("semctl(): error");
		exit(1);
	}

	//* Fork process and let child and parent perform operations.
	pid = fork();

	// Get shm and attach to it. Do it in main to avoid repeating code. Can be just copy pasted to child and parent and will work without problems.
	key_t shm_text_key, shm_num_key;
	int shmid_text, shmid_num;

	// Generate key to connect to shm_text
	if ((shm_text_key = ftok("q4.c", 'T')) == -1)
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

	// Generate key to connect to shm_num
	if ((shm_num_key = ftok("q4.c", 'N')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}

	// Create the shared mem segment if not created
	if ((shmid_num = shmget(shm_num_key, SHM_SIZE, SHM_PERM | IPC_CREAT)) == -1)
	{
		perror("shmget(): error");
		exit(1);
	}

	if (pid == -1)
	{
		perror("fork(): error");
		exit(1);
	}
	else if (pid == 0)
	{
		// Child
		char *shmptr_text, *shmptr_num;

		struct sembuf child_num;
		char str[SHM_SIZE];

		// Read the text segment written by parent!
		//* Entry Section (Allocate both resources)
		// Requires read lock first and then mutex lock
		child_num.sem_op = -1;
		child_num.sem_num = TEXT_SEG_LOCK;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		child_num.sem_op = -1;
		child_num.sem_num = MUTEX;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_text
		if ((shmptr_text = shmat(shmid_text, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Read data
		memset(str, '\0', SHM_SIZE);
		strncpy(str, shmptr_text, SHM_SIZE);
		printf("Child read %s from text segment\n", str);

		// Detach shm
		if (shmdt(shmptr_text) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//* Exit Section (Deallocate both resources)
		child_num.sem_op = 1;
		child_num.sem_num = TEXT_SEG_LOCK;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		// Free mutex and then the text segment lock
		child_num.sem_op = 1;
		child_num.sem_num = MUTEX;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		//* Write to num
		child_num.sem_num = MUTEX;
		child_num.sem_op = -1;
		child_num.sem_flg = 0;

		//* Entry Section (Allocate both resources)
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_num
		if ((shmptr_num = shmat(shmid_num, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Write data
		memset(str, '\0', SHM_SIZE);
		sprintf(str, "%d", NUM_MSG);
		printf("Child wrote %s to num segment\n", str);
		strncpy(shmptr_num, str, SHM_SIZE);

		// Detach shm
		if (shmdt(shmptr_num) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)

		// Free number lock as it has been written to.
		child_num.sem_op = 1;
		child_num.sem_num = NUM_SEG_LOCK;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		// Free mutex lock so parent can also write to either text or num segment
		child_num.sem_op = 1;
		child_num.sem_num = MUTEX;
		if (semop(semid, &child_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		return 0;
	}
	else
	{
		// Parent
		char *shmptr_text, *shmptr_num;
		struct sembuf parent_num;
		char str[SHM_SIZE];

		// Write to text
		parent_num.sem_num = MUTEX;
		parent_num.sem_op = -1;
		parent_num.sem_flg = 0;

		//* Entry Section (Allocate both resources)
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_num
		if ((shmptr_text = shmat(shmid_text, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Write data
		memset(str, '\0', SHM_SIZE);
		sprintf(str, "%s", TEXT_MSG);
		printf("Parent wrote %s to text segment\n", str);
		strncpy(shmptr_text, str, SHM_SIZE);

		// Detach shm
		if (shmdt(shmptr_text) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//* Exit Section (Deallocate both resources)
		parent_num.sem_op = 1;
		parent_num.sem_num = TEXT_SEG_LOCK;
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		parent_num.sem_op = 1;
		parent_num.sem_num = MUTEX;
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		// Write to text over! Now go to read num

		// First get read lock for text segment then get mutex
		parent_num.sem_num = NUM_SEG_LOCK;
		parent_num.sem_op = -1;

		//* Entry Section (Allocate both resources)
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		parent_num.sem_num = MUTEX;
		parent_num.sem_op = -1;

		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_text
		if ((shmptr_num = shmat(shmid_num, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Read data
		memset(str, '\0', SHM_SIZE);
		strncpy(str, shmptr_num, SHM_SIZE);
		printf("Parent read %s from num segment\n", str);

		// Detach shm
		if (shmdt(shmptr_num) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)
		parent_num.sem_op = 1;
		parent_num.sem_num = MUTEX;
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		parent_num.sem_op = 1;
		parent_num.sem_num = NUM_SEG_LOCK;
		if (semop(semid, &parent_num, 1) == -1)
		{
			perror("semop(): error");
			exit(1);
		}
		// Wait for child to finish
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid(): error");
			exit(1);
		}

		// Cleanup: Destroy shm and sem
		if (semctl(semid, 0, IPC_RMID) == -1)
		{
			perror("semctl");
			exit(1);
		}
		if (shmctl(shmid_text, IPC_RMID, NULL) == -1)
		{
			perror("shmctl(): error");
			exit(1);
		}
		if (shmctl(shmid_num, IPC_RMID, NULL) == -1)
		{
			perror("shmctl(): error");
			exit(1);
		}
		return 0;
	}
	return 0;
}