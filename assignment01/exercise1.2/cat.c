#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fileDescriptor;
	for (int i = 1; i < argc; i++)
	{
		if ((fileDescriptor = open(argv[i], O_RDONLY)) < 0)
		{
			printf("Could not find file %s", argv[i]);
		}
		else
		{
			char oneChar;
			while (read(fileDescriptor, &oneChar, 1))
			{
				write(STDOUT_FILENO, &oneChar, 1);
			}
			oneChar = '\n';
			write(STDOUT_FILENO, &oneChar, 1);
			close(fileDescriptor);
		}
	}
	return 0;
}