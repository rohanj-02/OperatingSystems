#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
// stat to see permissions
// Default is -t: implemented but remove files starting with a .
// IMplement sort for normal ls
// ls -a : print all including those starting with .

// int main(int argc, char *argv[])
// {

// 	char pwd[256];
// 	getcwd(pwd, sizeof(pwd));
// 	DIR *currDir;
// 	currDir = opendir(pwd);
// 	struct dirent *iterDir;

// 	if (currDir != NULL)
// 	{
// 		iterDir = readdir(currDir);
// 		while (iterDir != NULL)
// 		{
// 			printf("%s\n", iterDir->d_name);
// 			iterDir = readdir(currDir);
// 		}
// 		closedir(currDir);
// 	}
// 	else
// 	{
// 		perror("Could not open directory! ");
// 	}
// }

char *getPath(char *argv[], char *path)
{
	int index = 1;
	// ?GET path returns empty string when doing only ls.. Therefore it does not work
	if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "-ta") == 0 || strcmp(argv[1], "-at") == 0)
	{
		index = 2;
	}
	if (argv[index] != NULL)
	{
		if (argv[index][0] != '/')
		{
			char pwd[256];
			getcwd(pwd, sizeof(pwd));
			strcat(pwd, "/");
			strcat(pwd, argv[index]);
			strcpy(path, pwd);
		}
		else
		{
			strcpy(path, argv[index]);
		}
	}
	else
	{
		char pwd[256];
		getcwd(pwd, sizeof(pwd));
		strcpy(path, pwd);
	}
	return path;
}

int getList(DIR *currDir, char **fileNames)
{
	struct dirent *iterDir;
	int counter = 0;
	if (currDir != NULL)
	{
		iterDir = readdir(currDir);
		while (iterDir != NULL)
		{
			// printf("%s\n", iterDir->d_name);
			fileNames[counter] = malloc(256);
			strcpy(fileNames[counter], iterDir->d_name);
			counter++;
			// printf("%s\n", fileNames[counter - 1]);
			iterDir = readdir(currDir);
		}
		closedir(currDir);
	}
	else
	{
		perror("Could not open directory! ");
	}
	return counter;
}

static int compare(const void *first, const void *second)
{
	return strcmp(*(const char **)first, *(const char **)second);
}

int main(int argc, char *argv[])
{

	//TODO Figure why ls is not working..
	char path[256];
	getPath(argv, path);
	DIR *currDir;
	currDir = opendir(path);
	char *fileNames[256];
	int numDir = getList(currDir, fileNames);

	if (strcmp(argv[1], "-t") == 0)
	{
		for (int i = 0; i < numDir; i++)
		{
			if (fileNames[i][0] != '.')
			{
				printf("%s\n", fileNames[i]);
			}
		}
	}
	else if (strcmp(argv[1], "-a") == 0)
	{
		qsort(fileNames, numDir, sizeof(const char *), compare);
		for (int i = 0; i < numDir; i++)
		{
			printf("%s\n", fileNames[i]);
		}
	}
	else if ((strcmp(argv[1], "-ta") == 0) || (strcmp(argv[1], "-at") == 0))
	{
		for (int i = 0; i < numDir; i++)
		{
			printf("%s\n", fileNames[i]);
		}
	}
	else
	{
		qsort(fileNames, numDir, sizeof(const char *), compare);
		for (int i = 0; i < numDir; i++)
		{
			// if (fileNames[i][0] != '.')
			// {
			printf("%s\n", fileNames[i]);
			// }
		}
	}
}