#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define str_equal(a, b) (strcmp(a, b) == 0)
#define MAX_BUFFER_SIZE 1024

void print_error(char *err_msg)
{
	perror(err_msg);
	exit(1);
}

int check_advisory_locks(FILE *fptr)
{
}

int wait_for_close()
{
	while (1)
	{
		printf("Enter q to quit the program: ");
		char buffer[10];
		scanf("%s", buffer);
		if (buffer[0] == 'q')
		{
			return 0;
		}
	}
}

int read_from_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL)
	{
		print_error("fopen(): error");
	}

	check_advisory_locks(fptr); // get advisory lock

	// What does getc return on error?
	char ch = getc(fptr);
	while (ch != EOF)
	{
		printf("%c", ch);
		ch = getc(fptr);
	}

	printf("\n");
	wait_for_close();
	// wait for input

	if (fclose(fptr) != 0)
	{
		print_error("fclose(): error");
	}

	// release lock
	return 0;
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
		read_from_file(argv[2]);
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