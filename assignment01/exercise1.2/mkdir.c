#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if ((mkdir(argv[i], 00777)) == -1)
		{
			// perror(strerror(errno));
			perror("Error");
			// printf("The directory %s already exists!\n", argv[i]);
		}
	}
}