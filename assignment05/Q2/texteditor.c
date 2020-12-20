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

//TODO add print statement of Ctrl+q is quit and Ctrl + s is save
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

// Reads data from file and prints it to stdout
int read_from_file(char *filename)
{
	FILE *fptr;
	// Open file in read mode
	if ((fptr = fopen(filename, "r")) == NULL)
	{
		print_error("fopen(): error");
	}

	// Get the advisory lock and if failed then warn the user to get a shared lock
	if (get_advisory_locks(fptr) != 0)
	{
		if (warn_user(fptr) != 0)
		{
			exit(0);
		}
	}

	// Print the file on stdout
	char ch = getc(fptr);
	while (ch != EOF)
	{
		printf("%c", ch);
		ch = getc(fptr);
	}

	printf("\n");
	// Wait for the user to close the program to test the file locking capabilities of the editor
	wait_for_close();

	// Release locks and then close the file descriptor
	release_advisory_locks(fptr);

	if (fclose(fptr) != 0)
	{
		print_error("fclose(): error");
	}

	return 0;
}

// Writes data to file pointed by fptr
int write_file(FILE *fptr)
{
	// Get the advisory lock and if failed then warn the user to get a shared lock
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
		// Get input
		char ch = get_key_input();

		switch (ch)
		{
			//TODO Add backspace
		// If input = ctrl + q then quit the program
		case 0x1f & 'q':
			release_advisory_locks(fptr);
			flag = 0;
			return 0;
			break;
		// If input = ctrl + s then save the buffer and reinitialize it
		case 0x1f & 's':
			save_file(fptr, buffer);
			i = 0;
			memset(buffer, 0, MAX_BUFFER_SIZE);
			break;
		// Default: Print the character on stdout
		default:
			putchar(ch);
			break;
		}

		// Add character to buffer if buffer not overflown
		if (i < MAX_BUFFER_SIZE)
		{
			buffer[i] = ch;
			i++;
		}
	}

	return 0;
}

// Menu option of writing to file. It overwrites the given file.
int write_to_file(char *filename)
{
	FILE *fptr;
	// TODo change to read write mode so fopen() doesn't destroy file
	if ((fptr = fopen(filename, "w")) == NULL)
	{
		print_error("fopen(): error");
	}

	// Close fptr if write to file successful
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

// Menu option of appending text to file
int append_to_file(char *filename)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "a")) == NULL)
	{
		print_error("fopen(): error");
	}

	// Close fptr if write to file successful
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

int main(int argc, char **argv)
{
	// Usage of the program
	// ./run_editor.o read filename
	// ./run_editor.o append filename
	// ./run_editor.o write filename
	if (argc < 3)
	{
		fprintf(stderr, "Invalid arguments!\n");
		exit(1);
	}

	// Menu
	// Enter ctrl + q to stop process for write/append, and q to stop process for read
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