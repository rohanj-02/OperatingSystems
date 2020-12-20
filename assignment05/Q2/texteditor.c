#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define str_equal(a, b) (strcmp(a, b) == 0)

int check_advisory_locks(char *filename)
{
}

int read_to_file(char *filename)
{
}

int write_to_file(char *filename)
{
}

int append_to_file(char *filename)
{
}

int main(int argc, char **argv)
{
	// ./texteditor.o read "filename"
	// ./texteditor.o append "filename" list of strings
	// ./texteditor.o write "filename" list of strings
	if (argc < 3)
	{
		fprintf(stderr, "Invalid arguments!\n");
		exit(1);
	}
	// Menu
	// For any process: Enter q to stop process
	// For advisory locks: The file is open in another process, are you sure you want to modify its contents? (y/n)
	if (str_equal(argv[1], "read"))
	{
		printf("Read mode\n");
	}
	else if (str_equal(argv[1], "write"))
	{
		printf("Write mode. Replaces the content of the file\n");
	}
	else if (str_equal(argv[1], "append"))
	{
		printf("Appends the data to EOF!\n");
	}
	else
	{
		fprintf(stderr, "Invalid arguments!\n");
		exit(1);
	}
}