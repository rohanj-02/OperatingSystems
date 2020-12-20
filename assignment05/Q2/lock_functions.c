#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

int get_advisory_locks(FILE *fptr)
{
	int fd = fileno(fptr);
	if (flock(fd, LOCK_EX | LOCK_NB) == -1)
	{
		if (errno == EWOULDBLOCK)
		{
			return -1;
		}
		else
		{
			perror("flock(): error");
			exit(1);
		}
	}
	return 0;
}

int release_advisory_locks(FILE *fptr)
{
	printf("Release advisory locks\r\n");
	return 0;
}

int warn_user(FILE *fptr)
{
	printf("This file is already open in another editor! Do you want to continue? (y/n) ");
	char buffer[10];
	scanf("%s", buffer);
	int fd = fileno(fptr);

	if (buffer[0] == 'y')
	{
		if (flock(fd, LOCK_SH) == -1)
		{
			perror("flock(): error");
			exit(1);
		}
		printf("User given shared lock. Beware of editing as it may lead to a race error!\n");
		return 0;
	}
	else
	{
		return -1;
	}
}