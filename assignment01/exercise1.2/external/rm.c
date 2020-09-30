#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

extern void printDocs(char *);

bool isInternalFile(char *str)
{
	char pwd[256];
	char *internal[256] = {"/docs/cat.txt", "/docs/cd.txt", "/docs/date.txt", "/docs/echo.txt", "/docs/exit.txt", "/docs/history.txt", "/docs/ls.txt", "/docs/mkdir.txt", "/docs/pwd.txt", "/docs/rm.txt", "/external/cat", "/external/date", "/external/ls", "/external/mkdir", "/external/rm", "/external/cat.c", "/external/date.c", "/external/ls.c", "/external/mkdir.c", "/external/rm.c", "/external/Makefile", "/external/printDocs.c", "/main.c", "/cd.c", "/echo.c", "/main", "/Makefile"};
	// char *internal[256] = {"/test.txt"};
	for (int i = 0; i < 27; i++)
	{
		char forbidden[512];
		strcpy(forbidden, getenv("PWD"));
		strcat(forbidden, internal[i]);
		char given[512];
		getcwd(given, sizeof(given));
		strcat(given, "/");
		strcat(given, str);
		if (strcmp(forbidden, given) == 0)
		{
			return true;
		}
	}
	return false;
}

int main(int argc, char *argv[])
{
	bool isPrompt = false;

	if (strcmp(argv[1], "--help") == 0)
	{
		printDocs("rm.txt");
		return 0;
	}
	if ((strcmp(argv[1], "-i") == 0) || (strcmp(argv[1], "-id") == 0) || (strcmp(argv[1], "-di") == 0))
	{
		isPrompt = true;
	}
	if ((strcmp(argv[1], "-d") == 0) || (strcmp(argv[1], "-id") == 0) || (strcmp(argv[1], "-di") == 0))
	{
		for (int i = 2; i < argc; i++)
		{
			bool flag = false;
			if (isPrompt)
			{
				printf("Delete %s?(y/n) ", argv[i]);
				char x;
				scanf("%c", &x);
				flag = x == 'y';
				scanf("%c", &x);
			}
			else
			{
				flag = true;
			}
			if (isInternalFile(argv[i]))
			{
				flag = false;
				printf("You are not permitted to delete the shell!\n");
			}
			if (flag)
			{
				if (remove(argv[i]) == 0)
				{
					printf("Succesfully deleted %s\n", argv[i]);
				}
				else
				{
					char error[256];
					sprintf(error, "rm: cannot remove %s", argv[i]);
					perror(error);
				}
			}
		}
	}
	else
	{
		int startIndex = 1;
		if (isPrompt)
		{
			startIndex = 2;
		}
		for (int i = startIndex; i < argc; i++)
		{
			bool flag = false;
			if (isPrompt)
			{
				printf("Delete %s?(y/n) ", argv[i]);
				char x;
				scanf("%c", &x);
				flag = x == 'y';
				scanf("%c", &x);
			}
			else
			{
				flag = true;
			}
			if (isInternalFile(argv[i]))
			{
				flag = false;
				printf("You are not permitted to delete the shell!\n");
			}
			if (flag)
			{
				if (unlink(argv[i]) == 0)
				{
					printf("Succesfully deleted %s\n", argv[i]);
				}
				else
				{
					char error[256];
					sprintf(error, "rm: cannot remove %s", argv[i]);
					perror(error);
				}
			}
		}
	}

	return 0;
}