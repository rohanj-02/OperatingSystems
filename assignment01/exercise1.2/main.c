#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void getInput(char **inputArr)
{
	char x;
	int counter = 0;
	bool flag = true;
	while (x = getchar())
	{
		if (x == '\n')
		{
			break;
		}
		else if (x == 32)
		{
			counter++;
			flag = true;
		}
		else
		{
			if (flag)
			{
				flag = false;
				inputArr[counter][0] = x;
			}
			else
			{
				// inputArr[counter] += x;
				char str[2] = "\0";
				str[0] = x;
				strcat(inputArr[counter], str);
			}

			// strcpy(inputArr[counter], inputArr[counter] + x);
		}
	}
}

int main(int argc, char *argv[])
{
	/* 
	Plan:
		1. Make a parser for commands
		2. Define internal commands
		3. Read about exec() family manpage
		4. Make function to execute external commands
		5. Handle errors that can happen 
		6. Check how you can implement only the specific external commands and not all external commands
	*/
	// system("clear");
	char *x[20];
	for (int i = 0; i < 20; i++)
	{
		x[i] = (char *)malloc(100 * sizeof(char));
	}
	getInput(x);
	for (int i = 0; i < 20; i++)
	{
		printf(" %s\n", x[i]);
	}
	return 0;
}