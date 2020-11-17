#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_SIZE 501

//Sender
struct data
{
	long mtype;
	char input[MAX_SIZE];
};

int main(void)
{
	// struct data buffer;
	key_t key;
	int msqid;
	int fptr;
	char file_input[MAX_SIZE];

	fptr = open("./para2.txt", O_RDONLY);
	if (fptr == -1)
	{
		perror("open(): error");
		exit(1);
	}

	// Generate queue to get the same message queue for all processes
	if ((key = ftok("q2c.c", 'a')) == -1)
	{
		perror("ftok(): error");
		exit(1);
	}

	// Get the message queue
	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
	{
		perror("msgget(): error");
		exit(1);
	}

	// Read the file and tokenize it
	if (read(fptr, &file_input, MAX_SIZE - 1) == -1)
	{
		perror("read(): error");
		exit(1);
	}

	char *tokens = strtok(file_input, " ");

	// Send individual tokens on the message queue
	while (tokens != NULL)
	{
		struct data buffer;
		buffer.mtype = 1;
		strcpy(buffer.input, tokens);
		int len = sizeof(tokens);

		if (msgsnd(msqid, &buffer, len, 0) == -1)
		{
			perror("msgsnd(): error");
			// Do not return, atleast send the other tokens
		}
		tokens = strtok(NULL, " ");
	}

	// Close the message queue
	// if (msgctl(msqid, IPC_RMID, NULL) == -1)
	// {
	// 	perror("msgctl");
	// 	exit(1);
	// }
	return 0;
}