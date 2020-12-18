/*	Name: Rohan Jain
	Roll No.: 2019095 */

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
#define MQ_PERMS 0666
#define FILENAME "./para1.txt"

//Sender
struct data
{
	long mtype;
	char input[MAX_SIZE];
};

int main(void)
{
	key_t key;
	int msqid, read_bytes;
	char file_input[MAX_SIZE];

	int fptr = open(FILENAME, O_RDONLY);
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
	if ((msqid = msgget(key, MQ_PERMS | IPC_CREAT)) == -1)
	{
		perror("msgget(): error");
		exit(1);
	}

	// Read the file and tokenize it
	if ((read_bytes = read(fptr, &file_input, MAX_SIZE - 1)) == -1)
	{
		perror("read(): error");
		exit(1);
	}
	file_input[read_bytes] = '\0'; // read() does not add null terminator at end
	char *tokens = strtok(file_input, " ");

	// Send individual tokens on the message queue
	while (tokens != NULL)
	{
		struct data buffer;
		buffer.mtype = 1; // mtype 1 because sending from para 1
		strcpy(buffer.input, tokens);
		int len = strlen(tokens);

		if (msgsnd(msqid, &buffer, len, 0) == -1)
		{
			perror("msgsnd(): error");
			// Do not return, atleast send the other tokens
		}
		tokens = strtok(NULL, " ");
	}

	// Leave closing of queue to the receiver
	return 0;
}