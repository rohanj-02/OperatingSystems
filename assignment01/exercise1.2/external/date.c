#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern void printDocs(char *);

int main(int argc, char *argv[])
{
	if (argv[1] != NULL && strcmp(argv[1], "--help") == 0)
	{
		printDocs("date.txt");
		return 0;
	}
	time_t currTime = time(NULL);
	struct tm *currTimeStruct = localtime(&currTime);
	if (currTimeStruct == NULL)
	{
		perror("date: localtime error");
		exit(1);
	}
	char output[256];
	if (argv[1] == NULL)
	{
		if (strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "date: strftime failed!\n");
			exit(1);
		};
	}
	else if (strcmp(argv[1], "-R") == 0)
	{
		if (strftime(output, sizeof(output), "%a, %d %b %Y %T %z\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "date: strftime failed!\n");
			exit(1);
		};
	}
	else if (strcmp(argv[1], "-u") == 0)
	{
		currTimeStruct = gmtime(&currTime);
		if (currTimeStruct == NULL)
		{
			perror("date: gmtime error");
			exit(1);
		}
		if (strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "date: strftime failed!\n");
			exit(1);
		};
	}
	else if ((strcmp(argv[1], "-uR") == 0) || (strcmp(argv[1], "-Ru") == 0))
	{
		currTimeStruct = gmtime(&currTime);
		if (currTimeStruct == NULL)
		{
			perror("date: gmtime error");
			exit(1);
		}
		if (strftime(output, sizeof(output), "%a, %d %b %Y %T %z\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "date: strftime failed!\n");
			exit(1);
		}
	}
	else if (argv[1] == NULL || (strcmp(argv[1], "") == 0))
	{
		if (strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct) == 0)
		{
			fprintf(stderr, "date: strftime failed!\n");
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "date: %s flag not valid. Try --help to know more.\n", argv[1]);
	}

	for (int i = 0; output[i] != '\0'; i++)
	{
		write(STDOUT_FILENO, &output[i], 1);
	}
	return 0;
}