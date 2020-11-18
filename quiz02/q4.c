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
#define NUM_SEM 2

union semun
{
	int val;			   // for SETVAL
	struct semid_ds *buf;  //for IPC_STAT, IPC_SET
	unsigned short *array; // for GETALL, SETALL
};

int main(void)
{
	// shm is shared memory segment everywhere
	pid_t pid;
	key_t sem_key;
	int semid;

	// fprintf(stderr, "Init");

	// Initialise a named semaphore and then connect to it in each process, child and parent.
	if ((sem_key = ftok("q4.c", 'A')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}
	if ((semid = semget(sem_key, NUM_SEM, SEM_PERMS | IPC_CREAT)) == -1)
	{
		perror("semget(): error");
		exit(1);
	}

	// fprintf(stderr, "Get sem");

	// Set value of all semaphores to 1
	ushort init_val[] = {1, 1};
	union semun arg;
	arg.array = init_val;
	if (semctl(semid, 0, SETALL, arg.array) == -1)
	{
		perror("semctl(): error");
		exit(1);
	}
	// if (semctl(semid, 1, SETVAL, 1) == -1)
	// {
	// 	perror("semctl(): error");
	// 	exit(1);
	// }
	// fprintf(stderr, "Set init sem");
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

	// fprintf(stderr, "Set shm, before fork");
	//* Fork process and let child and parent perform operations.
	pid = fork();

	if (pid == -1)
	{
		perror("fork(): error");
		exit(1);
	}
	else if (pid == 0)
	{
		// Child
		char *shmptr_text, *shmptr_num;

		struct sembuf child_num[2];
		char str[SHM_SIZE];

		//* Write to num
		for (int i = 0; i < 2; i++)
		{
			child_num[i].sem_num = i;
			child_num[i].sem_op = -1;
			child_num[i].sem_flg = 0;
		}

		// fprintf(stderr, "Before getting lock");
		//* Entry Section (Allocate both resources)
		if (semop(semid, child_num, 2) == -1)
		{
			perror("semop(): error");
			exit(1);
		}
		// fprintf(stderr, "After getting lock");

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_num
		if ((shmptr_num = shmat(shmid_num, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Write data
		memset(str, '\0', SHM_SIZE);
		sprintf(str, "1000");
		printf("Child wrote %s to num segment\n", str);
		strncpy(shmptr_num, str, SHM_SIZE);

		//Detach shm
		if (shmdt(shmptr_num) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)
		for (int i = 0; i < 2; i++)
		{
			child_num[i].sem_op = 1;
		}
		if (semop(semid, child_num, 2) == -1)
		{
			perror("semop(): error");
			exit(1);
		}
		// Write to num over! Now go to read text
		for (int i = 0; i < 2; i++)
		{
			child_num[i].sem_op = -1;
		}

		//* Entry Section (Allocate both resources)
		if (semop(semid, child_num, 2) == -1)
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

		//Detach shm
		if (shmdt(shmptr_text) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)
		for (int i = 0; i < 2; i++)
		{
			child_num[i].sem_op = 1;
		}
		if (semop(semid, child_num, 2) == -1)
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
		struct sembuf parent_num[2];
		char str[SHM_SIZE];

		// Write to text
		for (int i = 0; i < 2; i++)
		{
			parent_num[i].sem_num = i;
			parent_num[i].sem_op = -1;
			parent_num[i].sem_flg = 0;
		}

		// fprintf(stderr, "Before getting lock parent");
		//* Entry Section (Allocate both resources)
		if (semop(semid, parent_num, 2) == -1)
		{
			perror("semop(): error");
			exit(1);
		}
		// fprintf(stderr, "After getting lock parent");

		//* Critical Section
		// Get pointer to shm at 0th position i.e. Attach to shm_num
		if ((shmptr_text = shmat(shmid_text, (void *)0, 0)) == (char *)(-1))
		{
			perror("shmat(): error");
			exit(1);
		}

		// Write data
		memset(str, '\0', SHM_SIZE);
		sprintf(str, "Parent");
		printf("Parent wrote %s to text segment\n", str);
		strncpy(shmptr_text, str, SHM_SIZE);

		//Detach shm
		if (shmdt(shmptr_text) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)
		for (int i = 0; i < 2; i++)
		{
			parent_num[i].sem_op = 1;
		}
		if (semop(semid, parent_num, 2) == -1)
		{
			perror("semop(): error");
			exit(1);
		}

		// Write to text over! Now go to read num
		for (int i = 0; i < 2; i++)
		{
			parent_num[i].sem_op = -1;
		}

		//* Entry Section (Allocate both resources)
		if (semop(semid, parent_num, 2) == -1)
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

		//Detach shm
		if (shmdt(shmptr_num) == -1)
		{
			perror("shmdt(): error");
			exit(1);
		}

		//*Exit Section (Deallocate both resources)
		for (int i = 0; i < 2; i++)
		{
			parent_num[i].sem_op = 1;
		}
		if (semop(semid, parent_num, 2) == -1)
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