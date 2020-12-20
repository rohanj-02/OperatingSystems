#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lock_functions.h"
#include "editor.h"
#include <termios.h>

#define str_equal(a, b) (strcmp(a, b) == 0)
#define MAX_BUFFER_SIZE 1024

void print_error(char *err_msg)
{
	perror(err_msg);
	exit(1);
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

int save_file(FILE *fptr, char *data)
{
	int length = strlen(data);
	for (int i = 0; i < length; i++)
	{
		fprintf(fptr, "%c", data[i]);
	}
	data = "";
	return 0;
}

int confirm_exit()
{
	return 0;
}

int read_from_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL)
	{
		print_error("fopen(): error");
	}

	if (get_advisory_locks(fptr) != 0)
	{
		if (warn_user() != 0)
		{
			exit(0);
		}
	}

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

	// todo RELEASE before fptr close or after? and same for getting it!
	release_advisory_locks(fptr);
	// release lock
	return 0;
}

int write_file(FILE *fptr)
{
	if (get_advisory_locks(fptr) != 0)
	{
		if (warn_user() != 0)
		{
			exit(0);
		}
	}

	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);
	int i = 0;
	int flag = 1;
	while (flag)
	{
		char ch = get_key_input();
		switch (ch)
		{
			//TODO Add backspace
		case 0x1f & 'q':
			if (fclose(fptr) != 0)
			{
				print_error("fclose(): error");
			}
			// Apparently no need of this??
			// release_advisory_locks(fptr);
			flag = 0;
			break;
		case 0x1f & 's':
			save_file(fptr, buffer);
			i = 0;
			memset(buffer, 0, MAX_BUFFER_SIZE);
			break;
		default:
			// printf("%c", ch);
			putchar(ch);
			// fprintf(stdout, "%c", ch);
			break;
		}
		if (i < MAX_BUFFER_SIZE)
		{
			buffer[i] = ch;
			i++;
		}
	}

	// release_advisory_locks(fptr);
	// release lock
	return 0;
}

int write_to_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "w")) == NULL)
	{
		print_error("fopen(): error");
	}
	write_file(fptr);
	return 0;
}

int append_to_file(char *filename)
{

	FILE *fptr;
	if ((fptr = fopen(filename, "a")) == NULL)
	{
		print_error("fopen(): error");
	}
	write_file(fptr);
}

//append save delete flock
int main(int argc, char **argv)
{
	// ./texteditor.o read "filename"
	// ./texteditor.o append "filename"
	// ./texteditor.o write "filename"
	if (argc < 3)
	{
		fprintf(stderr, "Invalid arguments!\n");
		exit(1);
	}
	// Menu
	// Enter ctrl + q to stop process for write/append, and q to stop process for read
	// For advisory locks: The file is open in another process, are you sure you want to modify its contents? (y/n)
	// Open append and write in termios
	// Read just like that
	if (str_equal(argv[1], "read"))
	{
		printf("Read mode\n");
		read_from_file(argv[2]);
	}
	else if (str_equal(argv[1], "write"))
	{
		printf("Write mode. Replaces the content of the file\n");
		write_to_file(argv[2]);
	}
	else if (str_equal(argv[1], "append"))
	{
		printf("Appends the data to EOF!\n");
		append_to_file(argv[2]);
	}
	else
	{
		fprintf(stderr, "Invalid arguments!\n");
		exit(1);
	}
	return 0;
}