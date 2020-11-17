#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define SOCK_PATH "socket_q3"
#define MAX_SIZE 501

// Client as it sends data to server

int main(void)
{
	int sock_descriptor, bytes_received, len, fptr;
	struct sockaddr_in remote;
	char word[MAX_SIZE];
	char file_input[MAX_SIZE];

	// Open the file para1.txt
	if (fptr = open("./para1.txt", O_RDONLY) == -1)
	{
		perror("open(): error");
		exit(1);
	}

	// Connect to a Datagram Socket, AF_UNIX for local storage?
	if ((sock_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket(): error");
		exit(1);
	}

	printf("Trying to connect...\n");

	// Understand these lines/
	remote.sin_family = AF_UNIX;		// The AF_UNIX socket family is used to communicate between processes on the same machine.
	strcpy(remote.sun_path, SOCK_PATH); // Connect to the socket present at SOCK_PATH
	len = strlen(remote.sun_path) + sizeof(remote.sin_family);

	// Connect the sockets to the socket_descriptor to the remote
	if (connect(sock_descriptor, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect(): error");
		exit(1);
	}

	printf("Connected.\n");

	// Read the file and tokenize it
	if (read(fptr, &file_input, MAX_SIZE) == -1)
	{
		perror("read(): error");
		exit(1);
	}

	char *tokens = strtok(file_input, " ");

	// Send individual tokens on the message queue
	while (tokens != NULL)
	{
		char str[MAX_SIZE];
		strcpy(str, tokens);
		len = sizeof(tokens);

		// Send the data through socket to server
		if (send(sock_descriptor, str, len, 0) == -1)
		{
			perror("send");
			exit(1);
		}
		if ((bytes_received = recv(sock_descriptor, str, MAX_SIZE, 0)) > 0)
		{
			// Everything fine, the server received the message
			printf("Received properly!");
		}
		else
		{
			if (bytes_received < 0)
			{
				perror("recv(): error");
			}
			else
			{
				printf("Server closed connection\n");
			}
			exit(1);
		}
		tokens = strtok(NULL, " ");
	}

	close(sock_descriptor);
	return 0;
}