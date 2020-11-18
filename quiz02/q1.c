#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 501

int isEscape(char *in, int index)
{
	if (index == 0)
	{
		return 0;
	}
	if (in[index - 1] != '\\')
	{
		return 0;
	}
	switch (in[index])
	{
	case 'a':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 't':
	case 'v':
	case '\\':
	case '"':
	case '?':
	case '\'':
	case '0':
		return 1;
	default:
		return 0;
	}
}

int main(void)
{
	int pipe1[2]; // Sends data from parent to child
	int pipe2[2]; // Sends data from child to parent
	pid_t pid;

	// Setup pipe
	if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
	{
		perror("pipe(): error");
		exit(1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork(): error");
		exit(1);
	}
	else if (pid == 0)
	{
		// Child
		close(pipe1[1]);
		close(pipe2[0]);

		char input[MAX_SIZE];

		// Read from pipe
		if (read(pipe1[0], &input, MAX_SIZE - 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}

		int i = 0;
		for (i = 0; input[i] != '\0'; i++)
		{
			if (!isEscape(input, i))
			{
				//Convert to Upper Case if alphabet
				if (input[i] >= 97 && input[i] <= 122)
				{
					input[i] -= 32;
				}
			}
		}

		// Write upper case to pipe from child to parent
		if (write(pipe2[1], &input, i) == -1)
		{
			perror("write(): error");
			exit(1);
		}

		close(pipe1[0]);
		close(pipe2[1]);
		exit(0);
	}
	else
	{
		// Parent
		close(pipe1[0]);
		close(pipe2[1]);

		char input[MAX_SIZE], output[MAX_SIZE];
		// printf("Enter character input: "); //Comment this later
		fgets(input, MAX_SIZE, stdin);

		// Write the input to the pipe from parent to child
		if (write(pipe1[1], &input, strlen(input)) == -1)
		{
			perror("write(): error");
			exit(1);
		}

		// Wait for child to convert to uppercase and write to the pipe and then read from pipe from child to parent.
		if (read(pipe2[0], &output, MAX_SIZE - 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}
		printf("%s", output);

		// To wait for child process to end so no zombie
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid(): error");
			exit(1);
		}

		// Close the rest of the pipes
		close(pipe1[1]);
		close(pipe2[0]);
	}
	return 0;
}