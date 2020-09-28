#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
	if (strcmp(input[1], "-L") == 0)
	{
		cd(input[2]);
	}
	else if (strcmp(input[1], "-P") == 0)
	{
		cd(input[2]);
	}
	else
	{
		cd(input[1]);
		// if (chdir(input[1]) != 0)
		// {
		// 	perror("cd Error");
		// }
		// else
		// {
		// 	if (getcwd(pwd, sizeof(pwd)) == NULL)
		// 	{
		// 		perror("getcwd() error");
		// 	}
		// 	else
		// 	{
		// 		printf("pwd is : %s\n", pwd);
		// 	}
		// }
	}
}
