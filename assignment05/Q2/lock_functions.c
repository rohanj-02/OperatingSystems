#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

// Get exclusive non blocking advisory lock on file and if cannot get then return -1
int get_advisory_locks(FILE *fptr)
{
	// Fileno returns the file descriptor from a FILE* struct object
	int fd = fileno(fptr);
	// LOCK_EX is for exclusive and LOCK_NB for non blocking
	if (flock(fd, LOCK_EX | LOCK_NB) == -1)
	{
		if (errno == EWOULDBLOCK) // EWOULDBLOCK is thrown when the file is locksed and LOCK_NB flag is selected
		{
			return -1; // Failure to acquire lock
		}
		else
		{
			perror("flock(): error");
			exit(1);
		}
	}
	return 0; //Lock acquired successfully
}

// Release the advisory locks present on the file
int release_advisory_locks(FILE *fptr)
{
	int fd = fileno(fptr);
	// LOCK_UN is for unlock
	if (flock(fd, LOCK_UN) == -1)
	{
		perror("flock(): error");
		exit(1);
	}

	return 0; //Lock released successfully
}

// Warn user and if they wish to process then get a shared lock on the file.
int warn_user(FILE *fptr)
{
	printf("This file is already open in another editor! Do you want to continue? (y/n) ");
	char buffer[10];

	scanf("%s", buffer);
	int fd = fileno(fptr);

	if (buffer[0] == 'y')
	{
		// LOCK_SH is for shared lock
		if (flock(fd, LOCK_SH) == -1)
		{
			perror("flock(): error");
			exit(1);
		}
		printf("User given shared lock. Beware of editing as it may lead to a race error!\n");
		return 0; // User wanted to continue and got the shared lock
	}
	else
	{
		return -1; // User doesn't want to continue
	}
}