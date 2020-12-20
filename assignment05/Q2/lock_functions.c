#include <unistd.h>
#include <stdio.h>

int get_advisory_locks(FILE *fptr)
{
	printf("Get advisory locks\r\n");
	return 0;
}

int release_advisory_locks(FILE *fptr)
{
	printf("Release advisory locks\r\n");
	return 0;
}

int warn_user()
{
	printf("This file is already open in another editor! Do you want to continue? (y/n) ");
	char buffer[10];
	scanf("%s", buffer);
	if (buffer[0] == 'y')
	{
		return 0;
	}
	else
	{

		return -1;
	}
}