#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int NUMBER_OF_ARGUMENTS = 10;

char **getArguments()
{
	bool isInput = true;
	char **input = (char **)calloc(10, sizeof(char *));
	int currentNumber = 0;
	while (isInput)
	{
		char x;
		int length = 0;
		int CURRENT_SIZE = 20;
		char *string = (char *)malloc(CURRENT_SIZE);
		bool flag = true;
		while (x = getchar())
		{
			if (!isInput)
			{
				break;
			}
			if (x == '"')
			{
				flag = !flag;
			}
			if (x == '\n')
			{
				isInput = false;
			}
			if ((x == ' ' && flag) || !isInput)
			{
				input[currentNumber] = string;
				currentNumber++;
				if (currentNumber >= NUMBER_OF_ARGUMENTS)
				{
					char **temp3 = realloc(input, 2 * NUMBER_OF_ARGUMENTS * sizeof(char *));
					NUMBER_OF_ARGUMENTS *= 2;
					input = temp3;
				}
				break;
			}
			else
			{
				char temp[2] = "\0";
				temp[0] = x;
				strcat(string, temp);
				length++;
				if (length >= CURRENT_SIZE)
				{
					char *temp2 = realloc(string, 2 * CURRENT_SIZE);
					CURRENT_SIZE *= 2;
					string = temp2;
				}
			}
		}
	}
	NUMBER_OF_ARGUMENTS = currentNumber;
	return input;
}

void executeCommands(char **input)
{
	if (isInternal(input[0]))
	{
		handleInternal(input);
	}
	else
	{
		handleExternal(input);
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
	while (1)
	{
		char **input = getArguments();
		for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++)
		{
			printf("Pointer:%p Value:%s Length:%ld\n", *input[i], input[i], strlen(input[i]));
		}
		free(input);
	}

	return 0;
}