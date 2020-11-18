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

//TODO Race condition?
//TODO One weird symbol between the two processes
//TODO add who is sending data para1 or para2 in both q2 and q3
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

	// int iter = 0;
	// struct data buffer;
	// buffer.mtype = 1; // mtype 1 because sending from para 1
	// memset(buffer.input, '\0', MAX_SIZE);
	// while (file_input[iter] != '\0')
	// {
	// 	char temp[2];
	// 	if (file_input[iter] != ' ')
	// 	{
	// 		temp[0] = file_input[iter];
	// 		temp[1] = '\0';
	// 		strcat(buffer.input, temp);
	// 	}
	// 	else
	// 	{
	// 		// strcat(buffer.input, '\0');
	// 		int len = strlen(buffer.input);
	// 		printf("Sent %s size %ld\n", buffer.input, strlen(buffer.input));
	// 		if (msgsnd(msqid, &buffer, len, 0) == -1)
	// 		{
	// 			perror("msgsnd(): error");
	// 			// Do not return, atleast send the other tokens
	// 		}
	// 		memset(buffer.input, '\0', MAX_SIZE);
	// 	}
	// 	iter++;
	// }

	// int len = strlen(buffer.input);
	// printf("Sent %s", buffer.input);
	// if (msgsnd(msqid, &buffer, len, 0) == -1)
	// {
	// 	perror("msgsnd(): error");
	// 	// Do not return, atleast send the other tokens
	// }

	char *tokens = strtok(file_input, " \n");

	// // Send individual tokens on the message queue
	while (tokens != NULL)
	{
		struct data buffer;
		buffer.mtype = 1; // mtype 1 because sending from para 1
		strcpy(buffer.input, tokens);
		int len = sizeof(tokens);
		// int len = strlen(buffer.input);

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