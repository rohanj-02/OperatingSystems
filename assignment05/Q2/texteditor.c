#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/file.h>

#include "lock_functions.h"

#define str_equal(a, b) (strcmp(a, b) == 0) // To check if strings are equal
#define MAX_BUFFER_SIZE 1024
typedef struct termios terminal_state;

// For printing error handling messages and exiting the program
void print_error(char *err_msg)
{
	perror(err_msg);
	exit(1);
}

// Hang the process by waiting for user input to demonstrate file locking with read mode
int wait_for_close()
{
	while (1)
	{
		printf("Enter q to quit the program: ");
		char buffer[10];
		scanf("%s", buffer);
		if (buffer[0] == 'q')
		{
			return 0; //User wants to quit
		}
	}
}

// Saves the data on the file character by character
int save_file(FILE *fptr, char *data)
{
	int length = strlen(data);
	for (int i = 0; i < length; i++)
	{
		if (fprintf(fptr, "%c", data[i]) < 1)
		{
			print_error("fprintf(): error");
		}
	}
	return 0; // Saved successfully
}

// Get one character of input from the terminal
char get_key_input()
{
	// termios used to get inputs such as crtl+q and ctrl+s for quit and save
	terminal_state original, modified;
	char ch = 0;

	// Get terminal attributes to restore after taking input
	if (tcgetattr(STDIN_FILENO, &original) == -1)
	{
		perror("tcgetattr(): error");
		exit(1);
	}
	if (tcgetattr(STDIN_FILENO, &modified) == -1)
	{
		perror("tcgetattr(): error");
		exit(1);
	}

	// Change terminal attributes such that:
	// ICANON: Byte by byte input instead of line by line input
	// ECHO: The terminal doesn't print to stdout whatever it receives to stdin
	// IXON: Disabling Ctrl + s and Ctrl + q
	modified.c_iflag &= ~(IXON);
	modified.c_lflag &= ~(ECHO | ICANON);

	// Set terminal attributes to modified attributes
	// TCSAFLUSH to ignore anything already present in the stdin buffer
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified) == -1)
	{
		perror("tcsetattr(): error");
		exit(1);
	}

	ch = getchar();

	// Restore terminal attributes to original
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1)
	{
		perror("tcsetattr(): error");
		exit(1);
	}

	return ch;
}

//
int read_from_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL)
	{
		print_error("fopen(): error");
	}

	if (get_advisory_locks(fptr) != 0)
	{
		if (warn_user(fptr) != 0)
		{
			exit(0);
		}
	}

	char ch = getc(fptr);
	while (ch != EOF)
	{
		printf("%c", ch);
		ch = getc(fptr);
	}

	printf("\n");
	wait_for_close();

	release_advisory_locks(fptr);

	if (fclose(fptr) != 0)
	{
		print_error("fclose(): error");
	}

	return 0;
}

int write_file(FILE *fptr)
{
	if (get_advisory_locks(fptr) != 0)
	{
		if (warn_user(fptr) != 0)
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
			release_advisory_locks(fptr);
			flag = 0;
			return 0;
			break;
		case 0x1f & 's':
			save_file(fptr, buffer);
			i = 0;
			memset(buffer, 0, MAX_BUFFER_SIZE);
			break;
		default:
			putchar(ch);
			break;
		}
		if (i < MAX_BUFFER_SIZE)
		{
			buffer[i] = ch;
			i++;
		}
	}
	return 0;
}

int write_to_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "w")) == NULL)
	{
		print_error("fopen(): error");
	}

	if (write_file(fptr) == 0)
	{
		if (fclose(fptr) == -1)
		{
			print_error("fclose(): error");
		}
	}
	else
	{
		printf("Error in writing to file!");
		return -1;
	}

	return 0;
}

int append_to_file(char *filename)
{

	FILE *fptr;
	if ((fptr = fopen(filename, "a")) == NULL)
	{
		print_error("fopen(): error");
	}

	if (write_file(fptr) == 0)
	{
		if (fclose(fptr) == -1)
		{
			print_error("fclose(): error");
		}
	}
	else
	{
		printf("Error in writing to file!");
		return -1;
	}

	return 0;
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