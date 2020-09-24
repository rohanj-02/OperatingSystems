#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int NUMBER_OF_ARGUMENTS = 10;
char **HISTORY;
int MAX_HISTORY = 10;
int CURRENT_HISTORY = 0;
bool IS_RUNNING = true;
int MAX_LENGTH = 512;

char **getArguments()
{
	bool isInput = true;
	char **input = (char **)calloc(10, sizeof(char *));
	char *totalCommand = (char *)malloc(200);
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
			else
			{
				char temp[2] = "\0";
				temp[0] = x;
				strcat(totalCommand, temp);
			}
			if (x == '\n')
			{
				isInput = false;
			}
			if (x == '"')
			{
				flag = !flag;
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
	HISTORY[CURRENT_HISTORY] = totalCommand;
	CURRENT_HISTORY++;
	if (CURRENT_HISTORY >= MAX_HISTORY)
	{
		MAX_HISTORY *= 2;
		char **temp = realloc(HISTORY, MAX_HISTORY * sizeof(char *));
		HISTORY = temp;
	}
	NUMBER_OF_ARGUMENTS = currentNumber;
	return input;
}

void showHistory()
{
	for (int i = 0; i < CURRENT_HISTORY; i++)
	{
		printf("Index:%d Command:%s\n", i + 1, HISTORY[i]);
	}
}

bool isInternal(char *command)
{
	return true;
}

void handleInternal(char **input)
{
	if (strcmp(input[0], "exit") == 0)
	{
		IS_RUNNING = false;
	}
	else if (strcmp(input[0], "history") == 0)
	{
		showHistory();
	}
	else if (strcmp(input[0], "cd") == 0)
	{
		char pwd[256];
		if (chdir(input[1]) != 0)
		{
			perror("chdir() error");
		}
		else
		{
			if (getcwd(pwd, sizeof(pwd)) == NULL)
			{
				perror("getcwd() error");
			}
			else
			{
				printf("pwd is : %s\n", pwd);
			}
		}
	}
	else if (strcmp(input[0], "pwd") == 0)
	{
		char pwd[256];
		if (getcwd(pwd, sizeof(pwd)) == NULL)
		{
			perror("getcwd() error");
		}
		else
		{
			printf("pwd is : %s\n", pwd);
		}
	}
	else if (strcmp(input[0], "echo") == 0)
	{
		for (int i = 1; i < NUMBER_OF_ARGUMENTS; i++)
		{
			printf("%s ", input[i]);
		}
		printf("\n");
	}
	else
	{
		printf("Bad Input!\n");
	}
}

void handleExternal(char **input)
{
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
	return;
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
	HISTORY = (char **)calloc(MAX_HISTORY, sizeof(char *));
	system("clear");
	// char command[MAX_LENGTH];
	while (IS_RUNNING)
	{
		printf("current directory: ");
		// if (!fgets(command, MAX_LENGTH, stdin))
		// {
		// 	break;
		// }
		// if (command[strlen(command) - 1] == '\n')
		// {
		// 	command[strlen(command) - 1] = '\0';
		// }
		// HISTORY[CURRENT_HISTORY] = command;
		// CURRENT_HISTORY++;
		// if (CURRENT_HISTORY >= MAX_HISTORY)
		// {
		// 	char **temp = realloc(HISTORY, 2 * MAX_HISTORY * sizeof(char *));
		// 	HISTORY = temp;
		// 	MAX_HISTORY *= 2;
		// }
		// printf("%s", command);
		char **input = getArguments();
		executeCommands(input);
		// for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++)
		// {
		// 	printf("Pointer:%p Value:%s Length:%ld\n", *input[i], input[i], strlen(input[i]));
		// }
		free(input);
	}
	free(HISTORY);

	return 0;
}