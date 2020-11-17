#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int pipe1[2]; //Sends data from parent to child
	int pipe2[2]; //Sends data from child to parent
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

		char input;

		//Read from pipe
		if (read(pipe1[0], &input, 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}

		//Convert to Upper Case if alphabet
		if (input >= 97 && input <= 122)
		{
			input -= 32;
		}

		//Write upper case to pipe from child to parent
		if (write(pipe2[1], &input, 1) == -1)
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

		char input, output;
		printf("Enter character input: "); //Comment this later
		scanf("%c", &input);

		// Write the input to the pipe from parent to child
		if (write(pipe1[1], &input, 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}

		// Wait for child to convert to uppercase and write to the pipe and then read from pipe from child to parent.
		if (read(pipe2[0], &output, 1) == -1)
		{
			perror("read(): error");
			exit(1);
		}
		printf("%c", output);

		// To wait for child process to end so no zombie
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("wait: error");
			exit(1);
		}

		// Close the rest of the pipes
		close(pipe1[1]);
		close(pipe2[0]);
	}
	return 0;
}