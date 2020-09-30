#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

extern void printDocs(char *);

void cd(char *path)
{
	if (path[0] != '/')
	{
		char pwd[256];
		getcwd(pwd, sizeof(pwd));
		strcat(pwd, "/");
		strcat(pwd, path);
		if (chdir(pwd) != 0)
		{
			char err[256];
			strcpy(err, "cd: ");
			strcat(err, path);
			perror(err);
		}
	}
	else
	{
		if (chdir(path) != 0)
		{
			char err[256];
			strcpy(err, "cd: ");
			strcat(err, path);
			perror(err);
		}
	}
	return;
}

void runcd(char **input)
{
	char pwd[256];
	if (input[1] != NULL)
	{
		// if (strcmp(input[1], "-L") == 0)
		// {
		// 	struct stat file_info;
		// 	lstat(input[2], &file_info);
		// 	if (S_ISLNK(file_info.st_mode))
		// 	{
		// 		printf("SYMBOLIC LINK! ");
		// 	}
		// ? One way is to hardcode PWD as global variable and use that for the current PWD instead of using getcwd()
		// 	cd(input[2]);
		// }
		if (strcmp(input[1], "-P") == 0)
		{
			if (input[2] != NULL)
			{
				cd(input[2]);
			}
			else
			{
				char *path = getenv("HOME");
				cd(path);
			}
		}
		else if (strcmp(input[1], "--help") == 0)
		{
			printDocs("cd.txt");
		}
		else
		{
			cd(input[1]);
		}
	}

	else
	{
		char *path = getenv("HOME");
		cd(path);
	}
}
