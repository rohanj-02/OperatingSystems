#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

extern void printDocs(char *);

// -m and -v
//TODO ERROR HANDLING Check  number and input format before conversion...
unsigned int getIntFromOctal(char *str)
{
	int i = 0, j = 0;
	unsigned int returnVal = 0;
	for (i = 0; str[i] != '\0'; i++)
		;
	int length = i;
	for (i = length - 1; i >= 0; i--)
	{
		returnVal += (int)((str[i] - 48) * (int)pow(8, j));
		j++;
	}
	return returnVal;
}

int main(int argc, char *argv[])
{
	bool isAlert = false;
	int startIndex = 1;
	mode_t mode = 0775;
	if (strcmp(argv[1], "--help") == 0)
	{
		printDocs("mkdir.txt");
		return 0;
	}
	if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "-mv") == 0) || (strcmp(argv[1], "-vm") == 0))
	{
		isAlert = true;
		startIndex = 2;
	}
	if ((strcmp(argv[1], "-m") == 0) || (strcmp(argv[1], "-mv") == 0) || (strcmp(argv[1], "-vm") == 0))
	{
		startIndex = 3;
		mode = getIntFromOctal(argv[2]);
	}

	for (int i = startIndex; i < argc; i++)
	{
		if ((mkdir(argv[i], mode)) == -1)
		{
			perror("mkdir");
		}
		else
		{
			if (isAlert)
			{
				printf("Create directory: %s\n", argv[i]);
			}
		}
	}
}