#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// TODO Add error handling, if a command has only one word and no spaces at the end then segfault.
// TODO
/* 
1. cd, pwd
Command checklist(Docs + Error Handling):
////1. history
////2. exit
3. cd Add ~ support
4. echo
////5. pwd
6. ls
7. rm
8. mkdir
9. date
10. cat

*/

extern void runcd(char **);
extern void echo(char **, int);
extern void printDocs(char *);

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
	char *internal[] = {"exit", "history", "cd", "pwd", "echo", "help"};
	for (int i = 0; i < 6; i++)
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
		printf("history: %d: Offset position out of range!\nTry '--help' for more information.\n", offset);
		return;
	}
	for (int i = offset - 1; i < CURRENT_HISTORY - 1; i++)
	{
		HISTORY[i] = HISTORY[i + 1];
	}
	CURRENT_HISTORY--;
}

void history(char **input)
{
	if (input[1] != NULL)
	{
		if (strcmp(input[1], "-c") == 0)
		{
			MAX_HISTORY = 10;
			CURRENT_HISTORY = 0;
			HISTORY = (char **)calloc(MAX_HISTORY, sizeof(char *));
			printf("history: History Cleared!\n");
		}
		else if (strcmp(input[1], "-d") == 0)
		{
			if (input[2] == NULL)
			{
				printf("history: -d requires a numerical offset to delete history.\nTry '--help' for more information.\n");
				return;
			}
			else if (!isNum(input[2]))
			{
				printf("history: %s: Offset position out of range!\nTry '--help' for more information.\n", input[2]);
				return;
			}
			deleteHistory(atoi(input[2]));
		}
		else if (strcmp(input[1], "--help") == 0)
		{
			printDocs("history.txt");
		}
		else
		{
			if (isNum(input[1]))
			{
				showHistory(atoi(input[1]));
			}
			else
			{
				if ((input[1][0] == '-') && (input[1][1] < '0' || input[1][1] > '9'))
				{
					printf("history: %s: Flag not supported.\nTry '--help' for more information.\n", input[1]);
				}
				else
				{
					printf("history: %s: Position out of range!\nTry '--help' for more information.\n", input[1]);
				}
			}
		}
	}
	else
	{
		showHistory(-1);
	}
}

void pwd(char **input)
{
	bool isP = true;
	if (input[1] != NULL)
	{
		if (strcmp(input[1], "--help") == 0)
		{
			isP = false;
			printDocs("pwd.txt");
		}
		else if (strcmp(input[1], "-L") == 0)
		{
			isP = false;
			printf("pwd: Not implemented yet\n");
		}
		else if (strcmp(input[1], "-P") == 0)
		{
			isP = true;
		}
		else
		{
			isP = false;
			printf("pwd: %s: Flag not supported!\nTry '--help' for more information!\n", input[1]);
		}
	}
	if (isP)
	{
		char pwd[256];
		if (getcwd(pwd, sizeof(pwd)) == NULL)
		{
			perror("pwd: getcwd() error");
		}
		else
		{
			printf("%s\n", pwd);
		}
	}
}

void handleInternal(char **input)
{
	if (strcmp(input[0], "exit") == 0)
	{
		if (input[1] != NULL)
		{
			if (strcmp(input[1], "--help") == 0)
			{
				printDocs("exit.txt");
			}
			else
			{
				if (isNum(input[1]))
				{
					printf("Exiting with status code %d\n", atoi(input[1]));
					exit(atoi(input[1]));
				}
				else
				{
					printf("exit: Status should be an integer!\nTry '--help' for more information!\n");
				}
			}
		}
		else
		{
			IS_RUNNING = false;
			printf("Exiting with status code 0\n");
		}
	}
	else if (strcmp(input[0], "history") == 0)
	{
		history(input);
	}
	else if (strcmp(input[0], "cd") == 0)
	{
		runcd(input);
	}
	else if (strcmp(input[0], "pwd") == 0)
	{
		pwd(input);
	}
	else if (strcmp(input[0], "echo") == 0)
	{
		echo(input, NUMBER_OF_ARGUMENTS);
	}
	else if (strcmp(input[0], "help") == 0)
	{
		printDocs("help.txt");
	}
	else
	{
		printf("internal: Shell Internal Command Error! This error should never be thrown\n");
	}
}

void handleExternal(char **input)
{
	pid_t process = fork();
	if (process == -1)
	{
		perror("shell: Could not spawn child process");
		return;
	}
	if (process == 0)
	{
		char path[512];
		strcpy(path, getenv("PWD"));
		strcat(path, "/external/");
		strcat(path, input[0]);
		if (execvp(path, input) == -1)
		{
			char errorMsg[256];
			sprintf(errorMsg, "Could not run command: %s\nTry 'help' for more information", input[0]);
			perror(errorMsg);
			exit(1);
		}
		exit(0);
	}
	else
	{
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("wait: error");
			exit(1);
		}
		return;
	}
}

void executeCommands(char **input)
{
	if (input[0] != NULL)
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
	HISTORY = (char **)calloc(MAX_HISTORY, sizeof(char *));
	system("clear");
	while (IS_RUNNING)
	{
		printDefaultPath();
		char **input = getArguments();
		executeCommands(input);
		free(input);
	}
	free(HISTORY);
	return 0;
}