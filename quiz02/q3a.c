#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

// Sender or Client
#define PORT_NAME "1500" // Common port for all
#define LOCALHOST "127.0.0.1"
#define MAX_SIZE 501

int main(int argc, char *argv[])
{
	int sock_descriptor, fptr, return_val;
	struct addrinfo hints, *res, *iter;
	char file_input[MAX_SIZE];

	// Setup hints to use getaddrinfo()
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// Generic code for IPv4 and IPv6
	hints.ai_socktype = SOCK_DGRAM; // Datagram Sockets

	// Get LL of struct addrinfo() corresponding to LOCALHOST PORT_NAME and hints.
	if ((return_val = getaddrinfo(LOCALHOST, PORT_NAME, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo(): error: %s\n", gai_strerror(return_val));
		exit(1);
	}

	// Dont assume first entry in LL is correct, loop through all and try to make a socket
	for (iter = res; iter != NULL; iter = iter->ai_next)
	{
		if ((sock_descriptor = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) != -1)
		{
			break;
		}
		perror("socket(): error");
	}

	if (iter == NULL)
	{
		fprintf(stderr, "Socket not created\n");
		exit(1);
	}

	// Socket succesfully created
	// Open file
	fptr = open("./para1.txt", O_RDONLY);
	if (fptr == -1)
	{
		perror("open(): error");
		exit(1);
	}

	// Read the file and tokenize it
	if (read(fptr, &file_input, MAX_SIZE) == -1)
	{
		perror("read(): error");
		exit(1);
	}

	char *tokens = strtok(file_input, " ");

	// Send individual tokens to the socket
	while (tokens != NULL)
	{
		int bytes_sent = 0;
		char str[MAX_SIZE];
		strcpy(str, tokens);
		int len = sizeof(tokens);

		if ((bytes_sent = sendto(sock_descriptor, str, len, 0, iter->ai_addr, iter->ai_addrlen)) < len)
		{
			if (bytes_sent == -1)
			{
				perror("talker: sendto");
				exit(1);
			}
			else
			{
				fprintf(stderr, "Did not send all the data to the socket!\n");
			}
		}

		tokens = strtok(NULL, " ");
	}

	// Cleanup and close socket
	freeaddrinfo(res);		// free the LL of struct addrinfo
	close(sock_descriptor); // close the socket

	return 0;
}
