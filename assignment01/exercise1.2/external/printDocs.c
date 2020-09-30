#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

extern void printDocs(char *);

void printDocs(char *str)
{
	int fileDescriptor;
	char *pwd;
	pwd = getenv("PWD");
	char copy[256];
	strcpy(copy, pwd);
	strcat(copy, "/docs/");
	strcat(copy, str);
	if ((fileDescriptor = open(copy, O_RDONLY)) < 0)
	{
		printf("Could not find file %s\n", copy);
	}
	else
	{
		char oneChar;
		while (read(fileDescriptor, &oneChar, 1))
		{
			write(STDOUT_FILENO, &oneChar, 1);
		}

		oneChar = '\n';
		write(STDOUT_FILENO, &oneChar, 1);
		close(fileDescriptor);
	}
}