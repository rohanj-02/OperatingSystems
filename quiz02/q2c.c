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

//Receiver
struct data
{
	long mtype;
	char input[MAX_SIZE];
};

int main(void)
{
	key_t key;
	int msqid, flag = 1;

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

	// Receive messages till \n\n is not received
	while (flag)
	{
		struct data buffer;
		memset(buffer.input, '\0', MAX_SIZE);

		if (msgrcv(msqid, &buffer, MAX_SIZE - 1, 0, 0) == -1)
		{
			perror("msgrcv(): error");
			exit(1);
		}
		ulong len = strlen(buffer.input);

		// Stop receving if \n\n is received
		if (len >= 2)
		{
			if (buffer.input[len - 1] == '\n' && buffer.input[len - 2] == '\n')
			{
				flag = 0;
				if (len != 2)
				{
					buffer.input[len - 2] = '\0';
					printf("%s from para%ld\n", buffer.input, buffer.mtype);
				}
			}
		}
		if (flag)
		{
			printf("%s from para%ld\n", buffer.input, buffer.mtype);
		}
	}

	// Close the message queue
	if (msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl(): error");
		exit(1);
	}

	return 0;
}