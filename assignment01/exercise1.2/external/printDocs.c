#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

void printDocs(char *str)
{
	int fileDescriptor;
	char *pwd;
	pwd = getenv("PWD");
	strcat(pwd, "/docs/");
	strcat(pwd, str);
	if ((fileDescriptor = open(pwd, O_RDONLY)) < 0)
	{
		printf("Could not find file %s", pwd);
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