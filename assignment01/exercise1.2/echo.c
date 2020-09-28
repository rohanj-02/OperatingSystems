#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void printEscape(char c)
{
	switch (c)
	{
	case 'a':
		printf("\a");
		break;
	case '\\':
		printf("\\");
		break;
	case 'e':
		printf("\e");
		break;
	case 'f':
		printf("\f");
		break;
	case 'n':
		printf("\n");
		break;
	case 'r':
		printf("\r");
		break;
	case 't':
		printf("\t");
		break;
	case 'v':
		printf("\v");
		break;
	}
}

char *preprocess(char *message, char **input, int number_of_args, bool isEscape)
{
	int messageCounter = 0;
	for (int i = 0; i < number_of_args; i++)
	{
		bool flag = false;
		for (int j = 0; input[i][j] != '\0'; j++)
		{
			if (flag)
			{
				if (input[i][j] == 'b')
				{
					messageCounter--;
				}
				else if (input[i][j] == 'c')
				{
					message[messageCounter] = '\0';
					j++;
					return message;
				}
				else
				{
					message[messageCounter] = '\\';
					messageCounter++;
					message[messageCounter] = input[i][j];
					messageCounter++;
				}

				flag = false;
			}
			else
			{
				if (input[i][j] == '\\' && isEscape)
				{
					flag = true;
				}
				else if (input[i][j] != '"')
				{
					message[messageCounter] = input[i][j];
					messageCounter++;
				}
			}
		}
		message[messageCounter] = ' ';
		messageCounter++;
	}
	message[messageCounter] = '\0';
	return message;
}

void printEcho(char **input, int number_of_args, bool isEscape)
{
	char message[1024] = "\0";
	preprocess(message, input, number_of_args, isEscape);
	bool flag = false;
	for (int i = 0; message[i] != '\0'; i++)
	{
		if (flag)
		{
			printEscape(message[i]);
			i++;
			flag = false;
		}
		if (message[i] == '\\' && isEscape)
		{
			flag = true;
		}
		else if (message[i] != '"')
		{
			printf("%c", message[i]);
		}
	}
	return;
}

void echo(char **input, int number_of_args)
{
	if (strcmp(input[1], "--help") == 0)
	{
		printf("Can use -n -e -E");
		return;
	}
	if (strcmp(input[1], "-n") == 0)
	{
		printEcho(&input[2], number_of_args - 2, false);
	}
	else if (strcmp(input[1], "-e") == 0)
	{
		printEcho(&input[2], number_of_args - 2, true);
		printf("\n");
	}
	else if (strcmp(input[1], "-E") == 0)
	{
		printEcho(&input[2], number_of_args - 2, false);
		printf("\n");
	}
	else
	{
		printEcho(&input[1], number_of_args - 1, false);
		printf("\n");
	}
	return;
}
