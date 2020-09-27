#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char pwd[256];
	getcwd(pwd, sizeof(pwd));
	DIR *currDir;
	currDir = opendir(pwd);
	struct dirent *iterDir;
	if (currDir != NULL)
	{
		iterDir = readdir(currDir);
		while (iterDir != NULL)
		{
			printf("%s\n", iterDir->d_name);
			iterDir = readdir(currDir);
		}
		closedir(currDir);
	}
	else
	{
		perror("Could not open directory! ");
	}
}