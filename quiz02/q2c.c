#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>

struct data
{
	long mtype;
	char input[500];
};

//Receiver
int main(void)
{
	struct data buffer;
	key_t key;
	int msqid;

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

	for (;;)
	{
		if (msgrcv(msqid, &buffer, 500, 0, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}
		printf("%s ", buffer.input);
	}

	return 0;
}