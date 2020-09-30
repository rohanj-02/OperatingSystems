#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#define length 256
extern void printDocs(char *);

int main(int argc, char *argv[])
{
	int fileDescriptor;
	int countLineNumber = 2;
	int startIndex = 1;
	bool isLineNumber = false;
	bool isLineEnd = false;
	if (strcmp(argv[1], "--help") == 0)
	{
		printDocs("cat.txt");
		return 0;
	}
	else if (strcmp(argv[1], "-E") == 0)
	{
		isLineEnd = true;
		startIndex = 2;
	}
	else if (strcmp(argv[1], "-n") == 0)
	{
		isLineNumber = true;
		startIndex = 2;
	}
	else if ((strcmp(argv[1], "-nE") == 0) || (strcmp(argv[1], "-En") == 0))
	{
		isLineEnd = true;
		isLineNumber = true;
		startIndex = 2;
	}
	if (isLineNumber)
	{
		char temp = '1';
		write(STDOUT_FILENO, &temp, 1);
		temp = '\t';
		write(STDOUT_FILENO, &temp, 1);
	}
	for (int i = startIndex; i < argc; i++)
	{
		if ((fileDescriptor = open(argv[i], O_RDONLY)) < 0)
		{
			printf("Could not find file %s", argv[i]);
		}
		else
		{
			char oneChar;
			while (read(fileDescriptor, &oneChar, 1))
			{
				if (oneChar == '\n')
				{
					if (isLineEnd)
					{
						char temp = '$';
						write(STDOUT_FILENO, &temp, 1);
					}
					write(STDOUT_FILENO, &oneChar, 1);
					if (isLineNumber)
					{
						int printLen = snprintf(NULL, 0, "%d", countLineNumber);
						char str[length];
						sprintf(str, "%d", countLineNumber);
						write(STDOUT_FILENO, &str, printLen);
						countLineNumber++;
						oneChar = '\t';
						write(STDOUT_FILENO, &oneChar, 1);
					}
				}
				else
				{
					write(STDOUT_FILENO, &oneChar, 1);
				}
			}

			oneChar = '\n';
			write(STDOUT_FILENO, &oneChar, 1);
			close(fileDescriptor);
		}
	}
	return 0;
}