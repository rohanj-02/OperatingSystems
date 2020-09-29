#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// TODO Add error handling, if a command has only one word and no spaces at the end then segfault.
// TODO if command has one word and space at end it works...
// TODO
/* 
1. cd, pwd
////2. history one more flag
////3. ls check -t (maybe implement -h or -r for reverse)
////	ls -t definitely wrong. In ls, convert to lowercase to compare.
4. Error handling when no space at end
5. Documentation and help flags.
*/

extern void runcd(char **);
extern void echo(char **, int);

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

void showHistory(int x)
{
	if (x == -1 || x >= CURRENT_HISTORY)
	{
		for (int i = 0; i < CURRENT_HISTORY; i++)
		{
			printf("%d\t%s", i + 1, HISTORY[i]);
		}
	}
	else
	{
		for (int i = CURRENT_HISTORY - 1; i >= CURRENT_HISTORY - x; i--)
		{
			printf("%d\t%s", i + 1, HISTORY[i]);
		}
	}
}

bool isInternal(char *command)
{
	char *internal[] = {"exit", "history", "cd", "pwd", "echo"};
	for (int i = 0; i < 5; i++)
	{
		if (strcmp(command, internal[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

bool isNum(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			return false;
		}
	}
	return true;
}

void deleteHistory(int offset)
{
	if (offset > CURRENT_HISTORY || offset <= 0)
	{
		printf("%d: Offset position out of range!\n", offset);
		return;
	}
	for (int i = offset - 1; i < CURRENT_HISTORY - 1; i++)
	{
		HISTORY[i] = HISTORY[i + 1];
	}
	CURRENT_HISTORY--;
}

void handleInternal(char **input)
{
	if (strcmp(input[0], "exit") == 0)
	{
		IS_RUNNING = false;
		printf("Shell exited! \n");
	}
	else if (strcmp(input[0], "history") == 0)
	{
		if (input[1] != NULL)
		{
			if (strcmp(input[1], "-c") == 0)
			{
				MAX_HISTORY = 10;
				CURRENT_HISTORY = 0;
				HISTORY = (char **)calloc(MAX_HISTORY, sizeof(char *));
				printf("History Cleared!\n");
			}
			else if (strcmp(input[1], "-d") == 0)
			{
				if (input[2] == NULL)
				{
					printf("-d requires a numerical offset to delete history. Try '--help' for more information.\n");
					return;
				}
				else if (!isNum(input[2]))
				{
					printf("%s: Offset position out of range!\n", input[2]);
					return;
				}
				deleteHistory(atoi(input[2]));
			}
			else if (strcmp(input[1], "--help") == 0)
			{
				printf("History implements a numeric infront of it and -c to clear\n");
				//TODO write documentation.
			}
			else
			{
				if (isNum(input[1]))
				{
					showHistory(atoi(input[1]));
				}
				else
				{
					printf("%s: Position out of range!\n", input[1]);
				}
			}
		}
		else
		{
			showHistory(-1);
		}
	}
	else if (strcmp(input[0], "cd") == 0)
	{
		runcd(input);
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
		echo(input, NUMBER_OF_ARGUMENTS);
	}
	else
	{
		printf("Bad Input!\n");
	}
}

void handleExternal(char **input)
{
	pid_t process = fork();
	if (process == -1)
	{
		perror("Could not spawn child process ");
	}
	if (process == 0)
	{
		char path[512];
		strcpy(path, getenv("PWD"));
		strcat(path, "/external/");
		strcat(path, input[0]);
		if (execvp(path, input) == -1)
		{
			perror("Could not run external command");
		}
		exit(0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		return;
	}
	printf("External command");
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

char *getSubstringPWD(char *pwd)
{
	int c = 0;
	for (int i = 0; pwd[i] != '\0'; i++)
	{
		if (pwd[i] == '/')
		{
			c++;
		}
		if (c == 3)
		{
			return &pwd[i];
		}
	}
	return NULL;
}

void printDefaultPath()
{
	printf("\033[1;32m");
	printf("%s:", getenv("USER"));
	printf("\033[1;34m");
	char pwd[256];
	getcwd(pwd, sizeof(pwd));
	char *temp = getSubstringPWD(pwd);
	if (temp == NULL)
	{
		printf("%s", pwd);
	}
	else
	{
		printf("~%s", temp);
	}
	printf("\033[0m");
	printf("$ ");
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
		// char pwd[256];
		// getcwd(pwd, sizeof(pwd));
		// printf("%s$ ", pwd);
		printDefaultPath();
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