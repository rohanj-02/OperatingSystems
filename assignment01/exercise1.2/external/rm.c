#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// -i -d -r to see

int main(int argc, char *argv[])
{
	bool isPrompt = false;

	if (strcmp(argv[1], "--help") == 0)
	{
		printf("Can implement -i and -d");
		//TODO Write documentation
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
			}
			else
			{
				flag = true;
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