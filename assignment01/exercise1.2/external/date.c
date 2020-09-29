#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (strcmp(argv[1], "--help") == 0)
	{
		printf("-u for utc time and -R for rfc 5322 format for eg.: Mon, 14 Aug 2006 02:34:56 -0600");
		//TODO Add documentation
		return 0;
	}
	time_t currTime = time(NULL);
	struct tm *currTimeStruct = localtime(&currTime);
	if (currTimeStruct == NULL)
	{
		perror("localtime error");
		return 0;
	}
	char output[256];
	if (strcmp(argv[1], "-R") == 0)
	{
		if (strftime(output, sizeof(output), "%a, %d %b %Y %T %z\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "strftime failed! ");
		};
	}
	else if (strcmp(argv[1], "-u") == 0)
	{
		currTimeStruct = gmtime(&currTime);
		if (currTimeStruct == NULL)
		{
			perror("gmtime error");
			return 0;
		}
		if (strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "strftime failed! ");
			return 0;
		};
	}
	else if ((strcmp(argv[1], "-uR") == 0) || (strcmp(argv[1], "-Ru") == 0))
	{
		currTimeStruct = gmtime(&currTime);
		if (currTimeStruct == NULL)
		{
			perror("gmtime error");
			return 0;
		}
		if (strftime(output, sizeof(output), "%a, %d %b %Y %T %z\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "strftime failed! ");
			return 0;
		}
	}
	else if (argv[1] == NULL || (strcmp(argv[1], "") == 0))
	{
		if (strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "strftime failed! ");
			return 0;
		}
	}
	else
	{
		fprintf(stderr, "date error: %s flag not valid. Try --help to know more.\n", argv[1]);
	}

	for (int i = 0; output[i] != '\0'; i++)
	{
		write(STDOUT_FILENO, &output[i], 1);
	}
	return 0;
}