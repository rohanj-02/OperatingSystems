#include <stdio.h>

// TODO Change code s.t. it cannot remove directory.. It needs a flag to do it.

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (remove(argv[i]) == 0)
		{
			printf("Succesfully deleted %s\n", argv[i]);
		}
		else
		{
			printf("File %s doesn't exist\n", argv[i]);
		}
	}

	return 0;
}